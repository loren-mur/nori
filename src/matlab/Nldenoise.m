function Nldenoise(f, r, k)
     %inputs, dat and datvar
    file_name = 'cbox_path_mats';
    dat = exrread(strcat(file_name, '.exr'));
    datvar = exrread(strcat(file_name, '_var.exr'));
    flt = zeros(size(dat));
    wgtsum = zeros(size(dat));
    box2 = ones(2);
    datvar = max(datvar, convn(datvar,box2, 'same'));

    %loop over neighbors
    for dx = -r:r
        for dy= -r:r
            %compute distance to neighbor (dx,dy)
            ngb = circshift(dat,[dx,dy]);
            ngbvar = circshift(datvar,[dx,dy]);
            %definition of d2 on the slides, non uniform variance
            d2pixel = (((dat - ngb).^2) - (datvar + min(ngbvar, datvar))) ./ ( eps + k^2 * (datvar + ngbvar));
            boxf = ones(f);
            %box filter
            d2patch = convn(d2pixel, boxf, 'same');
            wgt = exp(-max(0, d2patch));
            boxf1 = ones(f-1);
            %patch contribution
            wgt = convn(wgt, boxf1, 'same');
            flt = flt + wgt .* ngb;
            wgtsum = wgtsum + wgt;
        end
    end
    flt =flt./wgtsum;
    exrwrite(flt, strcat('cbox_path_mats', '_denoised.exr'));


end

       
