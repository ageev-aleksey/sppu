warning('off')
init1 = [-1.2 -1.85 -0.3];
init2 = [-4.29057, -0.54548, -0.43331];
init3 = [-2.78018, -0.06948, -0.56728];
init4 = [-4.7359   -0.0382   -0.7392];
value = [];
% for i=-0.5:0.2:0.5
%     for j=-0.5:0.2:0.5
%         for k=-0.5:0.2:0.5
%             d = init(3)+k;
%             if d > 0
%                 break;
%             end
%             value = [value; init(1)+i init(2)+j init(3)+k];
%         end
%     end
% end
value = [value; init1; init2; init3; init4];

opt = optimoptions('ga', 'InitialPopulationMatrix', value, 'PlotFcn','gaplotbestf');
opt.Display = 'iter';
opt.MaxGenerations = 10; 
UB = [0.0, 0.0, 0.0];
LB = [-5.0, -5.0, -5.0];
ga(@FitnesFunction, 3, [], [],[],[],LB,UB, [], opt);
