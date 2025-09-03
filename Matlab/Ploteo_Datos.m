%Ploteo datos 18-06

%plot(datoscontrol2.VarName1(1:3000));
velocidad_real = velocidad.VarName1;
largo = length(velocidad_real);

Ts = 2e-3;
tiempo = 0:Ts:(largo*Ts - Ts);


figure(1);
plot(tiempo, velocidad_real,'LineWidth',2);

hold on;
plot(Velocidad.time, Velocidad.signals.values,'LineWidth',2);
% xlim([0 1]);
% ylim([0 250]);
legend({'Experimental','Simulación'},'FontSize',12);
xlabel('t [seg]','FontSize',14);
ylabel('\omega_m [rad/s]','FontSize',14);


% u = 10.94*ones(largo,1);
% % u2 = 15.65*ones(largo-501,1);
% 
% %u = [u1; u2];
% u(1) = 0;
% 
% hold on;
% plot(tiempo,u);

% ylim([0 250]);
% legend({'Experimental','Simulación'},'Location','southeast','FontSize',12);



% hold on;
% sys = tf(tf3.num, tf3.den);
% step(9.51*sys);

% xlim([0 1]);
% step(15.65*sys,'-r');
% xlabel('t [seg]','FontSize',14);
% ylabel('\omega_m [rad/s]','FontSize',14);
% title('Respuesta a lazo abierto de un escalón de tensión');
% legend({'Experimental','Sistema Identificado'},'Location','southeast','FontSize',12);



%%

figure(2);

datos_voltaje = voltaje.VarName1;
largo_voltaje = length(datos_voltaje);
Ts2 = 3*Ts;
tiempo2 = 0:Ts2:(largo_voltaje*Ts2 - Ts2 );

%ceros = zeros(length(tiempo2)-largo_voltaje,1)';
%voltaje1 = voltaje.VarName1';
%datos_voltaje = [ceros voltaje1];

% hold on;
plot(tiempo2,datos_voltaje,'LineWidth',2);

hold on;
plot(accion_control.time, accion_control.signals.values,'LineWidth',2);

%xlim([0 1.5]);
% ylim([2 15]);
% legend({'Experimental','Simulación'},'FontSize',14);
% xlabel('t [seg]','FontSize',14);
% ylabel('Tensión [V]','FontSize',14);


%%
%Relacion entre el valor digital AnalogWrite y Va.

% digital = [50 100 150 200 255];
% tension_Va = [1 9.51 13.65 15.62 19.5];
% 
% plot(digital, tension_Va);
% 
% 
% %%
% u_control = ucontrol9.VarName1(400:500);
% largo = length(u_control);
% 
% suma = 0;
% 
% for i=1:largo;
%     
%     suma = suma + u_control(i);
%      
% end
% 
% promedio = suma/largo






















