delete(instrfindall);
s = serial('COM3', 'BaudRate', 115200, 'DataBits', 8, 'Parity', 'none');
fopen(s);

labels = zeros(100, 1);
values = zeros(100, 1);

h = plot(labels, values);

i = 0;

while 1
    try
        value = fscanf(s, '%f');
        
        labels = circshift(labels, -1);
        values = circshift(values, -1);
        labels(100) = i;
        values(100) = value(1);
        
        h.XData = labels;
        h.YData = values;
        
        i = i + 1;
        drawnow;
        pause(0.01);
    catch
        fclose(s);
    end
end