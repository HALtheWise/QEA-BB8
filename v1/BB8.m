function BB8
%% constants
m = 3;
g = 9.81;
l = 1;
I = 10;
friction = 0;

%% ode
initial_theta = .1;
initial_theta_dot = 0;
initial_w = 0;

options = [];

[T, U] = ode23s(@netFlows, 0:.001:10, [initial_theta; initial_theta_dot; initial_w], options);

theta1 = U(:,1);
thetadot1 = U(:,2);
omega1 = U(:,3);

function res = netFlows(T, U) 
       theta = U(1);
       theta_dot = U(2);
       theta_dot_dot = (-friction + (m*g*sin(theta)))/(m*l);
       omega_dot = (friction*l)/I;
       res = [theta_dot; theta_dot_dot; omega_dot];
end

normal_force = -m.*l.*thetadot1.^2 + m.*g.*cos(theta1);

%% plots
subplot(2,2,1)
plot(T, theta1, 'r')
title('Theta')

subplot(2,2,2)
plot(T, omega1, 'g')
title('Omega')

subplot(2,2,3)
plot(T, normal_force, 'k')
title('Normal Force')

subplot(2,2,4)
plot(T, theta1, 'r')
hold on;
plot(T, omega1, 'g')
plot(T, normal_force, 'k')

% display(normal_force(end))
% display(theta(end))

end