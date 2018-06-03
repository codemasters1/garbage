% 3 warstwy o danych liczbach neuronow
L1 = 2;
L2 = 2;
L3 = 1;

% parametr regularyzacji
lambda = 0;

% zestaw treningowy
A = [
    0.5 0.0 1.0;
    0.0 0.5 1.0;
    0.5 0.5 1.0;
    1.0 1.0 0.0;
];

X = A(:, 1:2);
y = A(:, 3);

% inicjalizacja wag i 'biasow' losowymi wartosciami, aby zapobiec tworzenia symetrii
nn_params = zeros(9, 1);

fprintf('\nTrenowanie sieci...\n')

for i=0:10
    options = optimset('MaxIter', 50);
    [nn_params, cost] = fmincg(@(p) siecKoszt(p, X, y, lambda), nn_params, options);
end

Theta1 = reshape(nn_params(1:6), 2, 3);
Theta2 = reshape(nn_params(7:9), 1, 3);

for i=1:size(y, 1)
    a1 = X(i,:)';
    z1 = Theta1 * [1; a1];
    a2 = sigmoid(z1);
    z2 = Theta2 * [1; a2];
    a3 = sigmoid(z2);

    x = X(i,:);

    fprintf('f(%f, %f) = %f\n', x(1), x(2), a3);
end
