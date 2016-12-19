objNum = 7;
[direct_data, indirect_data] = loadTrainData(objNum);

for i = 1:7
    nnwb = train_single_data(indirect_data{i});
    outputname = strcat('indirect_', int2str(i), '.mat');
    save(outputname, 'nnwb');
end

for i = 1:7
    nnwb = train_single_data(direct_data{i});
    outputname = strcat('direct_', int2str(i), '.mat');
    save(outputname, 'nnwb');
end