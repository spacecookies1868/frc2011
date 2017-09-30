function poly_constants = ProcessAccelrationData(filename, accel_duration, polyOrder)

data = csvread(filename);

data(:,2) = data(:,2) - data(1,2);

total_time = data(:,2);

accel_indices = find(data(:,1) == 1.0);
deccel_indices = find(((data(:,1) ~= 1.0) & (abs((data(:,3) + data(:,4)))) > 0));

accel_time = data(accel_indices,2);
accel_rps = data(accel_indices,3:4);

deccel_time = data(deccel_indices, 2);
deccel_rps = data(deccel_indices, 3:4);

figure;
hold on;
plot(accel_time, accel_rps(:,1), 'blue');
plot(accel_time, accel_rps(:,2), 'red');

figure;
hold on
plot(deccel_time, deccel_rps(:,1), 'blue');
plot(deccel_time, deccel_rps(:,2), 'red');

drive_time_indices = find (data(accel_indices,2) > 1.0);
accel_rps_drive_mean = mean(accel_rps(drive_time_indices, :))

driveEffeciency_left_over_right = accel_rps_drive_mean(1) / accel_rps_drive_mean(2)

decceleration_constant = mean(deccel_rps(1,1:2)) / (deccel_time(length(deccel_time)) - deccel_time(1))

if (accel_duration > 0.0)
   accel_duration_indices = find(accel_time < accel_duration); 
   accel_duration_time = accel_time(accel_duration_indices);
   accel_ducration_points = accel_rps(accel_duration_indices, :);
   accel_func = mean(accel_ducration_points,2);
   
   poly_constants = polyfit(accel_duration_time, accel_func, polyOrder)
   
   x = [0:0.001:accel_duration];
   y = polyval(poly_constants, x);
   
   figure;
   plot(accel_duration_time, accel_func, 'blue');
   hold on;
   plot(x,y,'red');
   
end

end