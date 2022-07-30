[sample_num, dim] = size(out.random_input)
random_input_str = '/'
for i = 1:sample_num
    random_input_str = append(num2str(random_input_str),',',num2str(out.random_input(i,2)));
end