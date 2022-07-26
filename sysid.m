clear

files = dir('./6v_step/*.csv');

for file = files'
    response_data = readtable(append(file.folder,'/',file.name));
    response_data = table2array(response_data);
    step.voltage = 12;
    step.input_time = 2;

    [sample_number, dim] = size(response_data);
    step_input = zeros(sample_number, dim);

    for i = 1:sample_number
        t = response_data(i,2);
        if t < step.input_time
            input_voltage = 0;
        else
            input_voltage = step.voltage;
        end
        step_input(i, 1) = t;
        step_input(i, 2) = input_voltage;
    end

    step.input = step_input(:, 2);
    step.response = response_data(:, 2);
end
