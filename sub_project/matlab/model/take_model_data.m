%Скрипт для сбора данных о моделировании управления 
% Пид контроллером
warning('off');
points = -0.7:0.05:0.7; %Сетка для перебора начальной и конечной позиции
pid = [-4.7359 -0.0382 -0.7392]; % Коэффициенты ПИД регулятора
out  = nan;
data = [];
index = 0;
for pbegin=points
    index = index + 1;
    for pend=points
        if pbegin == pend
            continue;
        end
        set_param('sdd_model/MechanicsOx/Begin_position','value', sprintf('%f', pbegin));
        set_param('sdd_model/End_position','value', sprintf('%f', pend));
        set_param('sdd_model/P', 'value', sprintf('%f', pid(1)));
        set_param('sdd_model/I', 'value', sprintf('%f', pid(2)));
        set_param('sdd_model/D', 'value', sprintf('%f', pid(3)));
        out = sim('sdd_model');
        
        step = out.tout(2) - out.tout(1);
        ms = (1/100)/step;
        json_struct = struct('time', out.tout(1:ceil(ms):length(out.tout)),...
                             'ox_pos', out.OxPositions,...
                             'ox_speed', out.OxSpeed,...
                             'duty_cycle', out.pwmControl,...
                             'begin_position', pbegin,...
                             'end_position', pend,...
                             'error_control', out.errorControl);
        data = [data; json_struct];
    end
end
save('simulation_data_with_error.mat', 'data');