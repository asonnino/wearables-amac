%************************************************************************
% Plot cubic spline interpolation
% thesis_project
% 
% Author:  Sonnino Alberto
% Version: 4.1
%
% ***********************************************************************

function plot_data(x,y)

% generate linera field
xx = linspace(x(1),x(end));

% compute cubic splines
yy = spline(x,y,xx);

% plot data
plot(x,y, '*k', 'MarkerSize',8, 'LineWidth',2);
hold on;
plot(xx,yy, '--k', 'LineWidth',1.5);

% make it beautiful
box off; grid on;
set(gca,'FontSize',12,'TickLabelInterpreter','none','XColor',...
    [0.501960813999176 0.501960813999176 0.501960813999176],...
    'YColor',...
    [0.501960813999176 0.501960813999176 0.501960813999176],'ZColor',...
    [0.501960813999176 0.501960813999176 0.501960813999176]...
);

end

% ***********************************************************************
