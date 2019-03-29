% https://github.com/CharleySchaefer/ZiltoidLIB
function demo_bisection()
  close all;
  % DEMO: bisection method applied to linear interpolation;
  %       See built-in function interp1()

  x=0:10;
  y1=sqrt(x);
  y0=2.9;

  % Ascending numbers
  [ind,f]=bisection(y1, y0);
  x1_bs=(1-f)*x( ind)+(f)*x( ind+1);
  y1_bs=(1-f)*y1(ind)+(f)*y1(ind+1);


  % Descending numbers
  y2=fliplr(y1);
  [ind,f]=bisection(y2, y0);
  x2_bs=(1-f)*x( ind)+(f)*x( ind+1);
  y2_bs=(1-f)*y2(ind)+(f)*y2(ind+1);

  % Plot
  subplot(2,1,1)
  plot(x,y1,'k', 'LineWidth', 3); hold on
  plot(x1_bs, y1_bs, '.r', 'MarkerSize', 15)
  xlabel('x')
  ylabel('y1')
  title('Ascending numbers')

  subplot(2,1,2)
  plot(x,y2,'k', 'LineWidth', 3); hold on
  plot(x2_bs, y2_bs, '.r', 'MarkerSize', 15)
  xlabel('x')
  ylabel('y2')
  title('Descending numbers')
end
