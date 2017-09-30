function tensorVal = ProcessRotationTest(filename)

wheel_radius = (3.863 / 2.0) * (2.54 / 100.0);
robot_width = 27 * 2.54 / 100.0;

data = csvread(filename);

dataSize = size(data);

timeDiff = data(dataSize(1),1) - data(1,1);
encoderDiff = data(dataSize(1), 4:5) - data(1,4:5);
gyroDiff = data(dataSize(1),6) - data(1,6);

time = data(:,1);
time_p = diff(time);

wheel_distance = 2 * pi * data(:,4:5);
wheel_speed = diff(wheel_distance);
gyro_val = data(:, 6);
gyro_p = diff(gyro_val);

wheel_speed_difference = wheel_speed(:,1) - wheel_speed(:,2);

tensor_vals = (wheel_speed_difference * robot_width) ./ gyro_p;

%figure;
%strName = sprintf('Left Motor %f Right Motor %f', data(1,2), data(1,3));
%title(strName);
%plot(tensor_vals);

tensorMean = mean(tensor_vals);

tensorVariance = abs(var(tensor_vals));

tensorVal = [data(1,2), data(1,3), abs(tensorMean), tensorVariance];

end