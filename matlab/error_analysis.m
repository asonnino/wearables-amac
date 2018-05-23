%************************************************************************
% Error Rate Analysis
% thesis_project
% 
% Author:  Sonnino Alberto
% Version: 4.1
%
% ***********************************************************************
close all;


% samples
er_range = [
    1 1 1  1  2  4  7  18 37 66;
    1 1 1  1  2  6  16 39 71 92;
    1 1 1  7  11 23 52 87 99 99;
    1 3 6 10  18 45 76 99 99 99
    ];
range = [1 2 3 4 5 6 7 8 9 10];

er_t = [
    1 1 4 1 1 12 85 83 79 81
    ];
t = [25 28 31 34 37 40 43 46 49 52];

er_int = [
    41 39 42 33 5 1 1 1 1 1
    ];
int = [
    0 0.2 0.4 0.6 0.8 1 1.2 1.4 1.6 1.8 
    ];


% plot
figure;
plot_data(range,er_range(1,:));
hold on;
plot_data(range,er_range(2,:));
hold on;
plot_data(range,er_range(3,:));
hold on;
plot_data(range,er_range(4,:));
xlabel('range [m]'); ylabel('error rate [%]');
legend('exp. data', 'cubic splines interpolation','Location', 'northwest');

figure;
plot_data(t,er_t);
xlabel('temperature [C]'); ylabel('error rate [%]');
legend('exp. data', 'cubic splines interpolation','Location', 'northwest');

figure;
plot_data(int,er_int);
xlabel('interferences [m]'); ylabel('error rate [%]');
legend('exp. data', 'cubic splines interpolation','Location', 'northeast');

% ***********************************************************************

