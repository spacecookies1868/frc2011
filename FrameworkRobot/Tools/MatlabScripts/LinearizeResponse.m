function data = LinearizeResponse(filename)

data = csvread(filename);

data

dataSize = size(data);
dataExtremes = [data(1,2:3), data(dataSize(1), 2:3)]
highestEncode = min(abs(dataExtremes))

data(:,2:3) = data(:,2:3) / highestEncode;

FILE = fopen('LinearizedMotorResponse.h', 'w');

fprintf(FILE, 'static const int NumMotorResponseRows=%d;\n',dataSize(1));
fprintf(FILE, 'static const double MotorResponse[%d][%d] = {\n', dataSize(1), dataSize(2));

for i=1:1:(dataSize(1)-1)
   fprintf(FILE, '\t{\t%f, \t%f, \t%f },\n', data(i,1), data(i,2), data(i,3)); 
end

fprintf(FILE, '\t{\t%f, \t%f, \t%f }\n', data(dataSize(1),1), data(dataSize(1),2), data(dataSize(1),3)); 

fprintf(FILE, '};');

fclose(FILE);

end