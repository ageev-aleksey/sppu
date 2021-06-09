Image = imread('binary.png');
Image = im2bw(Image, 0);
Image  = int8(Image);

 %plot(Image*255);
 
 [m,n]=size(Image);
 km = 0; kn = 0;
 cur = 1;
 Squares = zeros([50 1]);
 for i = 1:1:m
  for j = 1:1:n
    kn = j - 1;
    if kn <= 0 
      kn = 1;
      B = 0;
    else
      B = Image(i,kn); % Смотри рисунок 3 в статье
    end
    km = i - 1;
    if km <= 0 
      km = 1;
      C = 0;
    else
      C = Image(km,j); % Смотри рисунок 3 в статье
    end
    A = Image(i,j); % Смотри рисунок 3 в статье
    if A == 0  % Если в текущем пикселе пусто — то ничего не делаем
    elseif B == 0 && C == 0 % Если вокруг нашего пикселя пусто а он не пуст — то это повод подумать о новом объекте
        cur = cur + 1;
        Image(i,j) = cur;
        Squares(cur) = Squares(cur) + 1;
    elseif B ~=0 && C == 0
        Image(i,j) = B;
        Squares(B) = Squares(B) + 1;
    elseif B == 0 && C ~= 0
        Image(i,j) = C;
        Squares(C) = Squares(C) + 1;
    elseif B ~= 0 && C ~= 0   
         Squares(B) = Squares(B) + 1;
        if B == C
          Image(i,j) = B;
        else
          Image(i,j) = B;
          Image(Image == C) = B;
           Squares(B) = Squares(B) + Squares(C);
           Squares(C) = 0;
        end
    end
  end
 end
 
 [value, index] = max(Squares)
 
 % Вычисление центроида
 m00 = 0;
 m10 = 0;
 m01 = 0;
  for i=1:1:m
    for j = 1:1:n
        if Image(i, j) == index
            m00 = m00 + 1;
            m10 = m10 + i;
            m01 = m01 + j;
        end
    end
  end
  
  centroid = [m10/m00 m01/m00];

 