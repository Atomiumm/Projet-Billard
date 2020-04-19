clear;clc;

pos = %s;
x_min = %d;
x_max = %d;
y_min = %d;
y_max = %d;
ball_diam = %d;

plot(pos(:,1),pos(:,2),'r-',pos(:,3),pos(:,4),'y-',pos(:,5),pos(:,6),'k-');
axis ij

posleft = [pos;pos(end,:);pos(end,:)];
posright = [pos(1,:);pos(1,:);pos];
pos = [pos(1,:);pos;pos(end,:)];

n_frames = numel(pos)/6;

vel = (posright-posleft)/2;
acc = (posright-2*pos+posleft)/4;

velleft=[vel(2:end,:);vel(end,:)];
velright=[vel(1,:);vel(1:end-1,:)];

vel_bl = abs(vel)>1;
moving(:,1) = vel_bl(:,1) | vel_bl(:,2);
moving(:,2) = vel_bl(:,3) | vel_bl(:,4);
moving(:,3) = vel_bl(:,5) | vel_bl(:,6);

moving_ind = find(moving);
player_ball = min(mod(moving_ind,n_frames));
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

band_touch = (velleft(:,2*player_ball-1) < 0 & vel(:,2*player_ball-1) > 0 & (pos(:,2*player_ball-1)-x_min) < 2*ball_diam) | (velleft(:,2*player_ball-1) > 0 & vel(:,2*player_ball-1) < 0 & (-pos(:,2*player_ball-1)+x_max) < 3*ball_diam) | (velleft(:,2*player_ball) < 0 & vel(:,2*player_ball) > 0 & (pos(:,2*player_ball)-y_min) < 2*ball_diam) | (velleft(:,2*player_ball) > 0 & vel(:,2*player_ball) < 0 & (-pos(:,2*player_ball)+y_max) < 3*ball_diam);
ball_touch_a = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_a-1)) > 0 | abs(acc(:,2*ball_a)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_a-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_a)).^2) < 12*ball_diam^2;
ball_touch_b = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_b-1)) > 0 | abs(acc(:,2*ball_b)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_b-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_b)).^2) < 12*ball_diam^2;

band_touch=find(band_touch);
ball_touch_a = find(ball_touch_a);
ball_touch_b = find(ball_touch_b);

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