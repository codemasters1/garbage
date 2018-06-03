function [theta, J_history] = gradientDescentMulti(X, y, theta, alpha, num_iters)
    m = length(y);
    J_history = zeros(num_iters, 1);
    for iter = 1:num_iters
        n = theta;
        for z=1:length(theta)
            s = 0;
            for i=1:m
                s = s + (X(i,:) * theta - y(i)) * X(i,z);
            end
            
            n(z) = theta(z) - (alpha/m)*s;
        end
        
        theta = n;
        
        J_history(iter) = computeCost(X, y, theta);
        %J_history(iter), theta
    end
end
