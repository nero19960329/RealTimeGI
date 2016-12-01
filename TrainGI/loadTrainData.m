function [direct, indirect] = loadTrainData(cnt)
    direct = cell(1, cnt);
    indirect = cell(1, cnt);
    delimiterIn = ' ';
    headerlinesIn = 0;
    
    for i = 0:cnt - 1
        filename = strcat('data/direct_data_', int2str(i), '.txt');
        direct{i + 1} = importdata(filename, delimiterIn, headerlinesIn);
    end
    
    for i = 0:cnt - 1
        filename = strcat('data/indirect_data_', int2str(i), '.txt');
        indirect{i + 1} = importdata(filename, delimiterIn, headerlinesIn);
    end
end