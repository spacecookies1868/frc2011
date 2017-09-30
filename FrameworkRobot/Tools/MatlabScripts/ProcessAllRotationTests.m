function outData = ProcessAllRotationTests()
% copy-paste the following into your command window or your function

% first, you have to find the folder
folder = uigetdir; % check the help for uigetdir to see how to specify a starting path, which makes your life easier

% get the names of all files. dirListing is a struct array. 
dirListing = dir(folder);

numEntries=0;

% loop through the files and open. Note that dir also lists the directories, so you have to check for them.
for d = 1:length(dirListing)
    
    if ~dirListing(d).isdir
        fileName = fullfile(folder,dirListing(d).name); % use full path because the folder may not be the active path
        disp(fileName)
        numEntries = numEntries + 1;
        outData(numEntries, :) = ProcessRotationTest(fileName);
        
    end % if-clause
end % for-loop

overal_tensor = mean(outData(:,3));

FILE = fopen('RotationTensorLookup.h', 'w');

fprintf(FILE, 'double MeanTensorValue = %f;\n', overal_tensor);
fprintf(FILE, 'int NumRotationTensorEntries = %d;\n', numEntries);
fprintf(FILE, 'double RotationTensorLookupArray[%d][3] = {\n', numEntries);


for i=1:numEntries-1
    fprintf(FILE, '\t%f,\t%f,\t%f,\n', outData(i,1), outData(i,2), outData(i,3));
end

fprintf(FILE, '\t%f,\t%f,\t%f\n', outData(numEntries,1), outData(numEntries,2), outData(numEntries,3));

fprintf(FILE, '};\n');



fclose(FILE);

end