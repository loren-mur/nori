function Nldenoise(f, r, k)
    %inputs, dat and datvar
    %dat = exrread(strcat(file_name, '.exr'));
    %datvar = exrread(strcat(file_name, '_var.exr'));
    dat = exrread('immagine1.exr');
    datvar = var(dat);
    box2 = ones(2*2);
    %datvar = max(datvar, convn(datvar,box2, 'same'));
    flt = zeros(size(dat));
    wgtsum = zeros(size(dat));
    %loop over neighbors
    for dx = -r:r
        for dy= -r:r
            %compute distance to neighbor (dx,dy)
            ngb = circshift(dat,[dx,dy]);
            ngbvar = circshift(datvar,[dx,dy]);
            %definition of d2 on the slides, non uniform variance
            d2pixel = (((dat - ngb).^2) - (datvar + min(ngbvar, datvar))) ./ ( eps + k^2 * (datvar + ngbvar));
            boxf = ones(2*f+1);
            %box filter
            d2patch = convn(d2pixel, boxf, 'same');
            wgt = exp(-max(0, d2patch));
            boxf1 = ones(2*f-1);
            %patch contribution
            wgt = convn(wgt, boxf1, 'same');
            flt = flt + wgt .* ngb;
            wgtsum = wgtsum + wgt;
        end
    end
    %% Normalize and write result to a file
    flt =flt./wgtsum;
    exrwrite(flt, strcat('immagine1', '_denoised.exr'));
end

          

