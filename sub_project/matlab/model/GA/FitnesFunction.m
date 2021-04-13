function [J] = FitnesFunction(pid)
%begin_position = -0.5:0.2:0.5;
%end_position = -0.5:0.2:0.5;
begin_position = 0;
end_position=0.5;
x_w = 1;
x_cost = 0;
for b = begin_position
    for e = end_position
        set_param('sdd_model/MechanicsOx/Begin_position','value', sprintf('%f', b));
        set_param('sdd_model/End_position','value', sprintf('%f', e));
        set_param('sdd_model/P', 'value', sprintf('%f', pid(1)));
        set_param('sdd_model/I', 'value', sprintf('%f', pid(2)));
        set_param('sdd_model/D', 'value', sprintf('%f', pid(3)));
        out = sim('sdd_model');
    for i = 1:10:length(out.OxPositions)
        w = x_w;
        x = out.OxPositions(i);
        if out.tout(i) >= 0.2
            w = 10000;
        end
        %if abs(x) >= 0.5 
        %    w = 100;
        %end
        x_cost = x_cost + ((w*(e-x))^2);
    end
    end
end
J = x_cost;
%fprintf('J(%0.5f, %0.5f, %0.5f)=%0.5f\n', pid(1), pid(2), pid(3), J);
end

