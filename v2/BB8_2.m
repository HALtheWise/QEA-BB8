function BB8_2
%% constants
m1 = 1;
m2 = 4;
g = 9.81;
R = 1;
I = 1;

%% ode
initial_theta = 1;
initial_theta_dot = 0;
initial_xo = 0;
initial_xo_dot = 0;

options = [];

[T, U] = ode23(@netFlows, 0:.001:6, [initial_theta; initial_theta_dot; initial_xo; initial_xo_dot], options);

theta1 = U(:,1);
thetadot1 = U(:,2);
xo1 = U(:,3);
xodot1 = U(:,4);

function res = netFlows(~, U) 
       theta = U(1);
       theta_dot = U(2); % xo = U(3)
       xo_dot = U(4);
       f2 = 0;
       theta_dot_dot = (-1).*m2.^(-1).*R.^(-1).*(I+m1.*R.^2+I.*tan(theta).^2+m1.*R.^2.*tan(theta).^2+m2.*R.^2.*tan(theta).^2).^(-1).*(I.*f2+m1.*R.^2.*f2+m2.*R.^2.*f2+m2.*R.^2.*f2.*sec(theta)+(-1).*g.*I.*m2.*sec(theta).*tan(theta)+(-1).*g.*m1.*m2.*R.^2.*sec(theta).*tan(theta)+(-1).*g.*m2.^2.*R.^2.*sec(theta).*tan(theta)+I.*f2.*tan(theta).^2+m1.*R.^2.*f2.*tan(theta).^2+m2.*R.^2.*f2.*tan(theta).^2+m2.^2.*R.^3.*tan(theta).*theta_dot.^2);
       xo_dot_dot = R.^2.*(I.*cos(theta).^2+m1.*R.^2.*cos(theta).^2+I.*sin(theta).^2+m1.*R.^2.*sin(theta).^2+m2.*R.^2.*sin(theta).^2).^(-1).*(cos(theta).^2.*f2+cos(theta).^3.*f2+(-1).*g.*m2.*cos(theta).*sin(theta)+f2*sin(theta).^2+cos(theta).*f2.*sin(theta).^2+m2.*R.*cos(theta).^2.*sin(theta).*theta_dot.^2+m2.*R.*sin(theta).^3.*theta_dot.^2);
       res = [theta_dot; theta_dot_dot; xo_dot; xo_dot_dot];
end

%% plots
subplot(2,2,1)
plot(T, theta1, 'r')
title('Theta')

subplot(2,2,2)
plot(T, thetadot1, 'g')
title('Theta Prime')

subplot(2,2,3)
plot(T, xo1, 'k')
title('Position')

subplot(2,2,4)
plot(T, xodot1, 'b')
title('Velocity')

end