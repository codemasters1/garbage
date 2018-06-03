function [J, grad] = siecKoszt(nn_params, X, y, lambda)

% rozwiniecie wektora parametrow do osobnych macierzy
Theta1 = reshape(nn_params(1:6), 2, 3);
Theta2 = reshape(nn_params(7:9), 1, 3);

m = size(X, 1);

J = 0;
for i=1:m
    a1 = X(i,:)';
    z1 = Theta1 * [1; a1];
    a2 = sigmoid(z1);
    z2 = Theta2 * [1; a2];
    a3 = sigmoid(z2);
    J = J + (-y(i) * log(a3) - (1 - y(i)) * log(1 - a3));
end
J = J / m;

% dodanie wyrazu regularyzacji
t1 = Theta1(:,2:end);
t2 = Theta2(:,2:end);
J = J + sum([t1(:); t2(:)] .^ 2) * lambda / (2 * m);

% -------------------------------------------------------------

Delta1 = zeros(size(Theta1));
Delta2 = zeros(size(Theta2));

% dla kazdego sampla
for i=1:m
    % wyliczenie wyjscia sieci dla danego przypadku treningowego
    a1 = [1; X(i,:)'];
    z2 = Theta1 * a1;
    a2 = [1; sigmoid(z2)];
    z3 = Theta2 * a2;
    a3 = sigmoid(z3);
    
    % docelowe wyjscie
    target_output = y(i);
    
    % propagacja wsteczna
    delta3 = a3 - target_output;
    delta2 = (Theta2' * delta3) .* sigmoidGradient([1; z2]);
    
    Delta2 = Delta2 + delta3        * (a2');
    Delta1 = Delta1 + delta2(2:end) * (a1');
end

Theta1_grad = Delta1 / m;
Theta1_grad(:,2:end) = Theta1_grad(:,2:end) + lambda * Theta1(:,2:end) / m;
Theta2_grad = Delta2 / m;
Theta2_grad(:,2:end) = Theta2_grad(:,2:end) + lambda * Theta2(:,2:end) / m;

grad = [Theta1_grad(:); Theta2_grad(:)];

end
