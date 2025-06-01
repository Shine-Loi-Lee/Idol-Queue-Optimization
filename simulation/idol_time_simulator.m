wn_data = [
    0 1; 0 2; 0 3; 0 4; 0 5;
    1 1; 1 2; 1 3; 1 4; 1 5
];

alpha_vals = 0:0.1:10;
beta_vals = 0:0.1:10;
score_mat = zeros(length(beta_vals), length(alpha_vals));

best_score=-inf;

for i = 1:length(alpha_vals)
    alpha = alpha_vals(i);
    for j = 1:length(beta_vals)
        beta = beta_vals(j);

        value = alpha * wn_data(:,1) + beta * wn_data(:,2);
        
        [~, idx] = sort(value, 'descend');
        sorted=wn_data(idx,:);

        weights = (10:-1:1)';
        w_weighted_score = sum(sorted(:,1) .* weights * 10);
        n_weighted_score = sum(sorted(:,2) .* weights);

        score = w_weighted_score + n_weighted_score;
        score_mat(j, i) = score;

        if best_score<score
            best_score=score;
        end
    end
end

min_alpha_beta=inf;
min_alpha=0;
min_beta=0;

for i = 1:length(alpha_vals)
    alpha=alpha_vals(i);
    for j = 1:length(beta_vals)
        beta=beta_vals(j);
        if score_mat(j, i)==best_score &&...
                min_alpha_beta>alpha+beta &&...
                alpha*beta ~= 0
            min_alpha_beta=alpha+beta;
            min_alpha=alpha;
            min_beta=beta;
        end
    end
end

fprintf("Best alpha: %.2f\n", min_alpha)
fprintf("Best beta: %.2f\n", min_beta)

[ALPHA, BETA] = meshgrid(alpha_vals, beta_vals);
surf(ALPHA, BETA, score_mat)
xlabel('Alpha'); ylabel('Beta'); zlabel('Score');
title('Evaluation Score Surface');
