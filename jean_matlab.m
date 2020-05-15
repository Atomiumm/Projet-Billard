%% initialisation

clear;clc; close all

pos = %s;
ball_diam = %d;

%pos : n*6 matrix containing (xred, yred, xyel, yyel, xwhite, ywhite) for
%each n valid frame

%% detection of billard boundaries

x_min = min(min(pos(:,[1 3 5])));
x_max = max(max(pos(:,[1 3 5])));
y_min = min(min(pos(:,[2 4 6])));
y_max = max(max(pos(:,[2 4 6])));

%% plot of the trajectories

ScoreSheet = figure(1);

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
player_ball = ceil(min(moving_ind(mod(moving_ind-1,n_frames)==min(mod(moving_ind-1,n_frames))))/n_frames);
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

% creating comparative velocity to avoid both false positive and false
% negative during proximity test

vel_comp = sqrt(max(vel(:,2*player_ball-1).^2+vel(:,2*player_ball).^2,max(velleft(:,2*player_ball-1).^2+velleft(:,2*player_ball).^2,velright(:,2*player_ball-1).^2+velright(:,2*player_ball).^2)));

vel_comp_a = ball_diam + sqrt(max((vel(:,2*player_ball-1)-vel(:,2*ball_a-1)).^2+(vel(:,2*player_ball)-vel(:,2*ball_a)).^2,max((velleft(:,2*player_ball-1)-velleft(:,2*ball_a-1)).^2+(velleft(:,2*player_ball)-velleft(:,2*ball_a)).^2,(velright(:,2*player_ball-1)-velright(:,2*ball_a-1)).^2+(velright(:,2*player_ball)-velright(:,2*ball_a)).^2)));
vel_comp_b = ball_diam + sqrt(max((vel(:,2*player_ball-1)-vel(:,2*ball_b-1)).^2+(vel(:,2*player_ball)-vel(:,2*ball_b)).^2,max((velleft(:,2*player_ball-1)-velleft(:,2*ball_b-1)).^2+(velleft(:,2*player_ball)-velleft(:,2*ball_b)).^2,(velright(:,2*player_ball-1)-velright(:,2*ball_b-1)).^2+(velright(:,2*player_ball)-velright(:,2*ball_b)).^2)));

% a band is touched if a player ball acceleration is detected in the right
% direction while the ball is close enough to the corresponding band

% a ball is touched if both balls accelerates while being close enough

band_touch_left = velleft(:,2*player_ball-1) < 0 & velright(:,2*player_ball-1) > 0 & (pos(:,2*player_ball-1)-x_min) <= vel_comp;
band_touch_right = velleft(:,2*player_ball-1) > 0 & velright(:,2*player_ball-1) < 0 & (-pos(:,2*player_ball-1)+x_max) <= vel_comp;
band_touch_up = velleft(:,2*player_ball) < 0 & velright(:,2*player_ball) > 0 & (pos(:,2*player_ball)-y_min) <= vel_comp;
band_touch_down = velleft(:,2*player_ball) > 0 & velright(:,2*player_ball) < 0 & (-pos(:,2*player_ball)+y_max) <= vel_comp;
ball_touch_a = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_a-1)) > 0 | abs(acc(:,2*ball_a)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_a-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_a)).^2) < vel_comp_a.^2;
ball_touch_b = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_b-1)) > 0 | abs(acc(:,2*ball_b)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_b-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_b)).^2) < vel_comp_b.^2;

%% delete double count

band_touch_left_left = [band_touch_left(2:end);band_touch_left(end)];
band_touch_right_left = [band_touch_right(2:end);band_touch_right(end)];
band_touch_up_left = [band_touch_up(2:end);band_touch_up(end)];
band_touch_down_left = [band_touch_down(2:end);band_touch_down(end)];
ball_touch_a_left = [ball_touch_a(2:end);ball_touch_a(end)];
ball_touch_b_left = [ball_touch_b(2:end);ball_touch_b(end)];

band_touch_left(band_touch_left & band_touch_left_left) = 0;
band_touch_right(band_touch_right & band_touch_right_left) = 0;
band_touch_up(band_touch_up & band_touch_up_left) = 0;
band_touch_down(band_touch_down & band_touch_down_left) = 0;
ball_touch_a(ball_touch_a & ball_touch_a_left) = 0;
ball_touch_b(ball_touch_b & ball_touch_b_left) = 0;

band_touch=find(band_touch_left | band_touch_right | band_touch_up | band_touch_down);
ball_touch_a = find(ball_touch_a);
ball_touch_b = find(ball_touch_b);

%% showing band and ball touch on graph

plot(pos(band_touch,2*player_ball-1),pos(band_touch,2*player_ball),'*');
plot(pos(ball_touch_a,2*player_ball-1),pos(ball_touch_a,2*player_ball),'o');
plot(pos(ball_touch_b,2*player_ball-1),pos(ball_touch_b,2*player_ball),'o');

%% checking for winning condition : both balls are touched and 3 bands inbetween

win = 0;

if numel(ball_touch_a)==0 || numel(ball_touch_b)==0 || numel(band_touch)<3
    fprintf("win : \%d\\n",win);
    saveas(ScoreSheet,'ScoreSheet','pdf');
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
saveas(ScoreSheet,'ScoreSheet','pdf');

return