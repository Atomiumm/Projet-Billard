clear;clc; close all

pos = [[681,180,160,187,213,334];[681,180,188,256,213,334];[682,180,231,324,208,366];[681,180,288,334,197,353];[681,180,341,346,186,286];[681,180,390,357,175,221];[682,180,436,369,164,160];[682,180,482,382,153,101];[681,180,529,389,149,143];[682,180,577,381,149,184];[681,180,625,375,147,221];[681,180,677,369,145,254];[681,180,726,364,142,285];[681,180,706,348,140,317];[681,180,664,326,137,349];[681,180,628,306,135,382];[681,180,597,288,134,374];[681,180,568,270,135,351];[681,180,542,253,135,331];[681,180,517,235,135,313];[681,180,493,218,135,295];[681,180,468,201,135,278];[681,180,444,184,135,261];[681,180,420,167,135,243];[681,180,396,150,135,227];[681,180,372,133,135,210];[681,180,348,117,135,193];[681,180,324,100,135,176];[681,180,302,108,135,160];[681,180,278,118,135,143];[681,180,255,127,135,126];[681,180,232,137,135,111];[682,180,209,146,135,100];[681,180,186,155,136,112];[681,180,163,164,136,121];[681,180,140,173,136,130];[681,180,138,184,136,139];[681,180,155,198,136,148];[681,180,169,210,136,157];[681,180,181,223,136,166];[681,180,194,235,136,175];[681,180,206,248,135,183];[681,180,219,260,135,192];[681,180,231,272,135,200];[681,180,243,284,135,208];[681,180,256,296,135,217];[681,180,268,308,135,225];[681,180,280,320,135,233];[681,180,291,332,135,241];[681,180,302,344,135,249];[681,180,314,356,135,257];[681,180,326,367,134,264];[681,180,338,379,134,272];[681,180,349,390,134,279];[681,180,360,383,134,287];[681,180,372,376,133,294];[681,180,382,370,133,301];[681,180,394,364,133,308];[681,180,404,358,133,316];[681,180,414,352,133,323];[681,180,426,345,133,329];[682,180,436,339,133,336];[682,180,446,333,133,343];[682,180,456,326,132,349];[681,180,466,321,132,356];[681,180,476,315,132,362];[681,180,486,309,132,368];[681,180,496,303,132,374];[681,180,506,297,132,380];[681,180,516,291,131,386];[681,180,526,286,131,390];[681,180,536,280,131,387];[681,180,545,274,131,383];[681,180,554,269,131,381];[681,180,564,263,131,377];[681,180,572,258,132,374];[681,180,582,253,132,371];[681,180,592,248,131,369];[681,180,600,242,132,366];[681,180,610,236,132,363];[681,180,618,231,132,360];[681,180,627,226,132,358];[681,180,636,221,132,356];[681,180,644,216,132,353];[681,180,653,211,132,351];[681,180,662,206,132,349];[681,180,670,201,132,347];[681,180,678,196,132,345];[681,179,687,192,132,343];[681,177,696,190,132,341];[681,176,704,187,132,339];[681,174,712,185,132,337];[681,171,720,182,132,336];[681,169,728,180,132,334];[681,168,731,177,131,333];[681,167,726,174,656,91];[681,165,722,171,131,330];[681,163,718,169,663,96];[681,162,714,166,131,328];[680,161,709,163,131,327];[680,159,705,160,130,326];[680,159,700,158,130,325];[680,158,697,155,130,324];[680,157,693,153,130,324];[678,156,691,150,130,323];[676,156,690,147,130,323];[675,155,689,145,130,322];[673,155,688,142,131,322];[671,154,686,139,131,321];[670,154,685,137,131,321];[668,154,684,134,131,321];[667,154,683,132,131,321];[666,153,682,130,131,321];[665,153,680,128,131,321];[664,153,680,126,131,321];[663,153,679,124,131,321];[662,153,678,122,131,321];[661,153,677,120,131,321];[660,153,676,118,131,321];[659,153,675,117,131,321];[659,153,674,115,131,321];[658,153,674,114,131,321];[657,153,673,112,131,321];[657,153,672,111,131,321];[656,153,672,110,131,321];[656,153,670,109,131,321]];
ball_diam = 11;

x_min = min(min(pos(:,[1 3 5])));
x_max = max(max(pos(:,[1 3 5])));
y_min = min(min(pos(:,[2 4 6])));
y_max = max(max(pos(:,[2 4 6])));

ScoreSheet = figure(1);

hold on

plot(pos(:,1),pos(:,2),'color',[0.631 0.075 0.004],'lineStyle','--');
plot(pos(:,3),pos(:,4),'color',[0.929 0.761 0.016],'lineStyle','--');
plot(pos(:,5),pos(:,6),'color',[0.157 0.157 0.235],'lineStyle','--');
axis ij

line([x_min x_max],[y_min y_min]);
line([x_min x_max],[y_max y_max]);
line([x_min x_min],[y_min y_max]);
line([x_max x_max],[y_min y_max]);

posleft = [pos;pos(end,:);pos(end,:)];
posright = [pos(1,:);pos(1,:);pos];
pos = [pos(1,:);pos;pos(end,:)];

n_frames = numel(pos)/6;

vel = (posright-posleft)/2;
acc = (posright-2*pos+posleft)/4;

dist(1) = floor(sum(sqrt(vel(2:end-1,1).^2+vel(2:end-1,2).^2)));
dist(2) = floor(sum(sqrt(vel(2:end-1,3).^2+vel(2:end-1,4).^2)));
dist(3) = floor(sum(sqrt(vel(2:end-1,5).^2+vel(2:end-1,6).^2)));

velleft=[vel(2:end,:);vel(end,:)];
velright=[vel(1,:);vel(1:end-1,:)];

moving(:,1) = (pos(:,1)-pos(1,1)).^2 + (pos(:,2)-pos(1,2)).^2 >= 100;
moving(:,2) = (pos(:,3)-pos(1,3)).^2 + (pos(:,4)-pos(1,4)).^2 >= 100;
moving(:,3) = (pos(:,5)-pos(1,5)).^2 + (pos(:,6)-pos(1,6)).^2 >= 100;

moving_ind = find(moving);
player_ball = ceil(min(moving_ind(mod(moving_ind-1,n_frames)==min(mod(moving_ind-1,n_frames))))/n_frames);

if player_ball == 1
    player='red';
    ball_a = 2;
    ball_b = 3;
elseif player_ball == 2
    player='yellow';
    ball_a = 1;
    ball_b = 3;
else
    player='white';
    player_ball = 3;
    ball_a = 1;
    ball_b = 2;
end

vel_comp = sqrt(max(vel(:,2*player_ball-1).^2+vel(:,2*player_ball).^2,max(velleft(:,2*player_ball-1).^2+velleft(:,2*player_ball).^2,velright(:,2*player_ball-1).^2+velright(:,2*player_ball).^2)));

vel_comp_a = ball_diam + sqrt(max((vel(:,2*player_ball-1)-vel(:,2*ball_a-1)).^2+(vel(:,2*player_ball)-vel(:,2*ball_a)).^2,max((velleft(:,2*player_ball-1)-velleft(:,2*ball_a-1)).^2+(velleft(:,2*player_ball)-velleft(:,2*ball_a)).^2,(velright(:,2*player_ball-1)-velright(:,2*ball_a-1)).^2+(velright(:,2*player_ball)-velright(:,2*ball_a)).^2)));
vel_comp_b = ball_diam + sqrt(max((vel(:,2*player_ball-1)-vel(:,2*ball_b-1)).^2+(vel(:,2*player_ball)-vel(:,2*ball_b)).^2,max((velleft(:,2*player_ball-1)-velleft(:,2*ball_b-1)).^2+(velleft(:,2*player_ball)-velleft(:,2*ball_b)).^2,(velright(:,2*player_ball-1)-velright(:,2*ball_b-1)).^2+(velright(:,2*player_ball)-velright(:,2*ball_b)).^2)));

band_touch_left = velleft(:,2*player_ball-1) < 0 & velright(:,2*player_ball-1) > 0 & (pos(:,2*player_ball-1)-x_min) <= vel_comp;
band_touch_right = velleft(:,2*player_ball-1) > 0 & velright(:,2*player_ball-1) < 0 & (-pos(:,2*player_ball-1)+x_max) <= vel_comp;
band_touch_up = velleft(:,2*player_ball) < 0 & velright(:,2*player_ball) > 0 & (pos(:,2*player_ball)-y_min) <= vel_comp;
band_touch_down = velleft(:,2*player_ball) > 0 & velright(:,2*player_ball) < 0 & (-pos(:,2*player_ball)+y_max) <= vel_comp;
ball_touch_a = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_a-1)) > 0 | abs(acc(:,2*ball_a)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_a-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_a)).^2) < vel_comp_a.^2;
ball_touch_b = (abs(acc(:,2*player_ball-1)) > 0 | abs(acc(:,2*player_ball)) > 0) & (abs(acc(:,2*ball_b-1)) > 0 | abs(acc(:,2*ball_b)) > 0) & ((pos(:,2*player_ball-1)-pos(:,2*ball_b-1)).^2 + (pos(:,2*player_ball)-pos(:,2*ball_b)).^2) < vel_comp_b.^2;

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

plot(pos(band_touch,2*player_ball-1),pos(band_touch,2*player_ball),'sq','lineStyle','none','color',[0.000000,0.000000,0.000000]);
plot(pos(ball_touch_a,2*player_ball-1),pos(ball_touch_a,2*player_ball),'o','lineStyle','none','color',[0.000000,0.000000,0.000000]);
plot(pos(ball_touch_b,2*player_ball-1),pos(ball_touch_b,2*player_ball),'o','lineStyle','none','color',[0.000000,0.000000,0.000000]);

win = 0;

if numel(ball_touch_a) && numel(ball_touch_b) && numel(band_touch)>=3
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
end

if numel(ball_touch_a) && numel(ball_touch_b)
    balls_touched_t = "2 balls touched";
elseif numel(ball_touch_a) || numel(ball_touch_b)
    balls_touched_t = "1 ball touched";
else
    balls_touched_t = "No ball touched";
end

if numel(band_touch) >= 2
    band_touched_t = sprintf("%d bands touched",numel(band_touch));
elseif numel(band_touch) == 1
    band_touched_t = "1 band touched";
else
    band_touched_t = "No band touched";
end

if win
    win_mess = "You won. You're wonderful <3";
else
    win_mess = "You lost. You litte shit.";
end

text_to_display = {sprintf("Scores for player %s",player),balls_touched_t,band_touched_t,sprintf("dist(r) = %dpx",dist(1)),sprintf("dist(y) = %dpx",dist(2)),sprintf("dist(w) = %dpx",dist(3)),win_mess};
x_text = x_min + [1 13 13 1 7 13 1]*(x_max-x_min)/18;
y_text = y_max + [14 14 43 72 72 72 43];
text(x_text,y_text,text_to_display);

time_t = floor(clock);
scoresheet_title = sprintf('Scores Sheet - F2 - %s %dh%dmin%ds',date,time_t(4:6));
sgtitle(scoresheet_title)

axis([x_min-5 x_max+5 y_min-5 y_max+100]);
set(gca,'visible','off');
set(gca,'xtick',[]);

set(gcf, 'PaperUnits', 'centimeters', 'Units', 'centimeters');
set(gcf, 'PaperSize', [29.7 21]);
set(gcf, 'Position', [0.5 0.5 28.7 20]);

saveas(ScoreSheet,'ScoreSheet','pdf');
open('ScoreSheet.pdf');

clc;clear;

return