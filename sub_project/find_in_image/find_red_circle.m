img = imread('-224 8.jpg');
%% Модификация гистограмы RGB по красному каналу
red = double(img(:, :, 1))/255;
green = double(img(:, :, 2))/255;
blue = double(img(:, :, 3))/255;
ix = [0, 0.4, 1];
iy = [0, 0, 1];
ired = (interp1(ix, iy, red));
iimg = cat(3, ired, green, blue);
%iimg = cat(3, red, green, blue);
hsv_img = rgb2hsv(iimg);
s = size(img);
for i=1:s(1)
    for j=1:s(2)
        if ~(hsv_img(i, j, 1) < 0.1 || hsv_img(i, j, 1) > 0.8)
            hsv_img(i, j, 3) = 0;
        end
    end
end

res = imbinarize(hsv_img(:, :, 3));
p = regionprops(res);
imshow(img);
hold
plot(p.Centroid(1), p.Centroid(2), "r*");