function nnwb = train_single_data(data)
    train_data = data(:, 1:15)';
    train_label = data(:, 16:18)';

    nn = feedforwardnet([20 10]);
    nn = train(nn, train_data, train_label);

    wb = getwb(nn);
    [b, IW, LW] = separatewb(nn, wb);

    nnwb = cell(1, 10);
    nnwb{1} = IW{1};
    nnwb{2} = b{1};
    nnwb{3} = LW{2,1};
    nnwb{4} = b{2};
    nnwb{5} = LW{3,2};
    nnwb{6} = b{3};
    nnwb{7} = nn.inputs{1}.range(:, 1);
    nnwb{8} = nn.inputs{1}.range(:, 2);
    nnwb{9} = nn.outputs{3}.range(:, 1);
    nnwb{10} = nn.outputs{3}.range(:, 2);
    
    view(nn);
end