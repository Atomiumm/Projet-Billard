function win=jean_matlab(pos,x_min,x_max,y_min,y_max,ball_diam)

%pos : n*6 matrix containing (xred, yred, xyel, yyel, xwhite, ywhite) for
%each n valid frame

%% plot of the trajectories

plot(pos(:,1),pos(:,2),'r-',pos(:,3),pos(:,4),'y-',pos(:,5),pos(:,6),'k-');

%% derivation of position

posleft = [pos;pos(end,:);pos(end,:)];
posright = [pos(1,:);pos(1,:);pos];
pos = [pos(1,:);pos;pos(end,:)];

vel = (posright-posleft)/2;
acc = (posright-2*pos+posleft)/4;

%% detection of the first moving ball to detect player ball

vel_bl = abs(vel)>1;
moving(:,1) = vel_bl(:,1) || vel_bl(:,2);
moving(:,2) = vel_bl(:,3) || vel_bl(:,4);
moving(:,3) = vel_bl(:,5) || vel_bl(:,6);

moving = find(moving);
player_ball = min(mod(moving,3));

if player_ball == 1 %change conditions to consider machine error
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

band_touch = (acc(:,2*player_ball) > 1 && (pos(:,2*player_ball)-x_min) < ball_diam) || (acc(:,2*player_ball) < -1 && (-pos(:,2*player_ball)+x_max) < 2*ball_diam) || (acc(:,2*player_ball+1) > 1 && (pos(:,2*player_ball+1)-y_min) < ball_diam) || (acc(:,2*player_ball+1) < -1 && (-pos(:,2*player_ball+1)+y_max) < 2*ball_diam);
ball_touch_a = (abs(acc(:,2*player_ball)) > 1 || abs(acc(:,2*player_ball+1)) > 1) && (abs(acc(:,2*ball_a)) > 1 || abs(acc(:,2*ball_a+1)) > 1) && ((pos(:,2*player_ball)-pos(:,2*ball_a)).*(pos(:,2*player_ball)-pos(:,2*ball_a)) + (pos(:,2*player_ball+1)-pos(:,2*ball_a+1)).*(pos(:,2*player_ball+1)-pos(:,2*ball_a+1))) < ball_diam*ball_diam;
ball_touch_b = (abs(acc(:,2*player_ball)) > 1 || abs(acc(:,2*player_ball+1)) > 1) && (abs(acc(:,2*ball_b)) > 1 || abs(acc(:,2*ball_b+1)) > 1) && ((pos(:,2*player_ball)-pos(:,2*ball_b)).*(pos(:,2*player_ball)-pos(:,2*ball_b)) + (pos(:,2*player_ball+1)-pos(:,2*ball_b+1)).*(pos(:,2*player_ball+1)-pos(:,2*ball_b+1))) < ball_diam*ball_diam;

band_touch=find(band_touch);
ball_touch_a = find(ball_touch_a);
ball_touch_b = find(ball_touch_b);

%% checking for winning condition : both balls are touched and 3 bands inbetween

win = 0;

if numel(ball_touch_a)==0 || numel(ball_touch_b)==0 || numel(band_touch)<3
    return
end

if max(ball_touch_a) < min(ball_touch_b)
    band_touch = band_touch(band_touch>max(ball_touch_a) && band_touch<min(ball_touch_b));
    if numel(band_touch)>=3
        win = 1;
    end
else
    band_touch = band_touch(band_touch>max(ball_touch_b) && band_touch<min(ball_touch_a));
    if numel(band_touch)>=3
        win = 1;
    end
end

end