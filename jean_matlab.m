%% initialisation

clear;clc;

pos = %s;
ball_diam = %d;

%pos : n*6 matrix containing (xred, yred, xyel, yyel, xwhite, ywhite) for
%each n valid frame

%%

x_min = min(pos([1 3 5],:));
x_max = max(pos([1 3 5],:));
y_min = min(pos([2 4 6],:));
y_max = max(pos([2 4 6],:));
%% plot of the trajectories

figure(1)

hold on

plot(pos(:,1),pos(:,2),'r-',pos(:,3),pos(:,4),'y-',pos(:,5),pos(:,6),'k-');
axis ij

line([x_min x_max],[y_min y_min]);
line([x_min x_max],[y_max y_max]);
line([x_min x_min],[y_min y_max]);
line([x_max x_max],[y_min y_max]);

%% detection of the first moving ball to detect player ball, and creation of arrays of interest

posleft = [pos;pos(end,:);pos(end,:)];
posright = [pos(1,:);pos(1,:);pos];
pos = [pos(1,:);pos;pos(end,:)];

n_frames = numel(pos)/6; %not real frames but osef

vel = (posright-posleft)/2;
acc = (posright-2*pos+posleft)/4;

velleft=[vel(2:end,:);vel(end,:)];
velright=[vel(1,:);vel(1:end-1,:)];

moving(:,1) = (pos(:,1)-pos(1,1)).^2 + (pos(:,2)-pos(1,2)).^2 >= 100;
moving(:,2) = (pos(:,3)-pos(1,3)).^2 + (pos(:,4)-pos(1,4)).^2 >= 100;
moving(:,3) = (pos(:,5)-pos(1,5)).^2 + (pos(:,6)-pos(1,6)).^2 >= 100;

moving_ind = find(moving);
player_ball = ceil(min(moving_ind(mod(moving_ind,n_frames)==min(mod(moving_ind,n_frames))))/n_frames);
fprintf("player ball : \%d\\n",player_ball);

if player_ball == 1
    ball_a = 2;
    ball_b = 3;
elseif player_ball == 2
    ball_a = 1;
    ball_b = 3;
else
    player_ball = 3;
    ball_a = 1;
    ball_b = 2;
end

%% logical arrays to identify the frames where player ball touches the bands and other balls

% a band is touched if a player ball acceleration is detected in the right
% direction while the ball is close enough to the corresponding band

% a ball is touched if both balls accelerates while being close enough

%a ajouter : tests de proximité dépendants de la vitesse

band_touch_vert = (velleft(:,2*player_ball-1) < 0 & velright(:,2*player_ball-1) > 0 & (pos(:,2*player_ball-1)-x_min) <= 2*ball_diam) | (velleft(:,2*player_ball-1) > 0 & velright(:,2*player_ball-1) < 0 & (-pos(:,2*player_ball-1)+x_max) < 3*ball_diam);
band_touch_hrzt = (velleft(:,2*player_ball) < 0 & velright(:,2*player_ball) > 0 & (pos(:,2*player_ball)-y_min) < 2*ball_diam) | (velleft(:,2*player_ball) > 0 & velright(:,2*player_ball) < 0 & (-pos(:,2*player_ball)+y_max) < 3*ball_diam);
ball_touch_a = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_a-1)) > 0 | abs(acc(:,2*ball_a)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_a-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_a)).^2) < 12*ball_diam^2;
ball_touch_b = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_b-1)) > 0 | abs(acc(:,2*ball_b)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_b-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_b)).^2) < 12*ball_diam^2;

%% delete double count

band_touch_vert_left = [band_touch_vert(2:end);band_touch_vert(end)];
band_touch_vert_hrzt_left = [band_touch_hrzt(2:end);band_touch_hrzt(end)];
ball_touch_a_left = [ball_touch_a(2:end);ball_touch_a(end)];
ball_touch_b_left = [ball_touch_b(2:end);ball_touch_b(end)];

band_touch_vert(band_touch_vert & band_touch_vert_left) = 0;
band_touch_hrzt(band_touch_hrzt & band_touch_hrzt_left) = 0;
ball_touch_a(ball_touch_a & ball_touch_a_left) = 0;
ball_touch_b(ball_touch_b & ball_touch_b_left) = 0;

band_touch=find(band_touch_vert | band_touch_hrzt);
ball_touch_a = find(ball_touch_a);
ball_touch_b = find(ball_touch_b);

%% checking for winning condition : both balls are touched and 3 bands inbetween

win = 0;

if numel(ball_touch_a)==0 || numel(ball_touch_b)==0 || numel(band_touch)<3
    fprintf("win : \%d\\n",win);
    return
end

if max(ball_touch_a) < min(ball_touch_b)
    band_touch = band_touch(band_touch>max(ball_touch_a) & band_touch<min(ball_touch_b));
    if numel(band_touch)>=3
        win = 1;
    end
else
    band_touch = band_touch(band_touch>max(ball_touch_b) & band_touch<min(ball_touch_a));
    if numel(band_touch)>=3
        win = 1;
    end
end

fprintf("win : \%d\\n",win);

return