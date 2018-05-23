function generate_samples(n)

a = n - n / 20;
b = n + n / 20;
r = (b-a).*rand(49,1) + a;
r 


end