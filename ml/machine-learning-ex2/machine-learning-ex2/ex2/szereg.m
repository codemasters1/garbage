function a = szereg(n)
    a = 0;
    for i=2:n
        a = a + ((-1)^i * 2^i) / log(i);
    end
end