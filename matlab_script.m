clear;clc; close all

pos = [[657,153,668,100,131,321];[657,152,667,100,131,321];[657,153,667,100,130,321];[657,153,667,100,130,321];[657,153,667,100,130,321];[657,152,667,100,130,321];[657,152,667,100,130,321];[657,152,667,100,130,321];[656,152,667,100,130,321];[656,152,667,100,130,321];[656,152,667,100,130,321];[656,152,667,100,130,321];[656,152,667,100,130,321];[656,152,667,114,130,321];[646,162,677,160,130,321];[624,177,698,194,130,321];[605,192,716,227,131,321];[589,205,732,261,131,321];[573,219,725,295,131,321];[557,231,716,332,131,321];[540,244,708,368,131,321];[524,257,699,388,131,321];[508,270,682,360,131,321];[492,283,666,335,131,321];[477,295,650,312,131,321];[461,308,634,293,131,321];[445,320,620,275,131,321];[429,333,605,257,131,321];[414,344,590,239,131,321];[398,357,576,221,130,321];[383,369,561,203,130,321];[368,381,547,185,130,321];[353,389,532,168,130,321];[342,381,518,150,130,321];[330,375,504,133,130,321];[320,369,490,116,130,321];[308,363,476,100,130,321];[297,357,468,108,130,321];[286,352,460,118,130,321];[276,346,452,127,130,321];[265,340,444,135,130,321];[254,334,436,143,130,321];[244,329,428,151,130,321];[234,323,420,159,130,321];[224,317,412,167,130,321];[213,313,404,175,130,321];[203,307,396,183,130,321];[193,301,388,191,130,321];[183,296,381,198,130,321];[173,291,374,206,131,321];[163,285,366,214,131,321];[153,280,358,221,131,321];[144,275,351,229,131,321];[134,270,344,236,131,321];[132,266,336,243,131,321];[138,261,330,250,131,321];[143,257,322,258,131,321];[149,252,316,265,131,321];[154,247,308,272,131,321];[159,243,302,278,130,321];[164,239,294,285,130,321];[169,235,288,292,130,321];[175,231,282,299,130,321];[179,227,274,305,130,321];[184,223,268,312,130,321];[188,219,262,318,130,321];[193,215,255,325,130,321];[198,211,249,331,130,321];[202,208,242,338,130,321];[206,205,236,344,130,321];[211,201,230,350,130,321];[215,197,224,356,130,321];[220,194,218,362,130,321];[224,190,212,368,130,321];[228,187,206,374,130,321];[232,184,200,379,130,321];[235,181,194,385,130,321];[240,178,188,390,130,321];[243,174,184,387,131,321];[246,171,179,384,131,321];[250,169,176,381,131,321];[254,166,171,378,131,321];[257,163,167,376,131,321];[260,160,163,373,131,321];[264,157,160,370,131,321];[267,155,156,367,131,321];[270,152,152,365,131,321];[273,149,148,362,131,321];[276,148,145,360,131,321];[278,145,141,358,130,321];[281,143,138,355,130,321];[284,141,135,353,130,321];[288,138,132,350,130,321];[290,136,128,348,130,321];[292,134,130,346,130,321];[294,133,131,345,130,321];[298,130,133,342,130,321];[300,128,134,341,130,321];[301,127,136,339,130,321];[303,125,138,337,130,321];[305,124,139,336,130,321];[308,121,140,334,131,321];[310,121,142,332,130,321];[312,120,144,331,131,321];[313,117,144,330,131,321];[315,116,146,328,131,321];[316,115,146,327,131,321];[318,113,148,326,131,321];[319,112,148,325,131,321];[320,111,149,323,131,321];[322,110,150,322,131,321];[323,109,150,322,130,321];[324,108,152,320,131,321];[325,107,152,320,130,321];[326,106,152,319,130,321];[327,106,152,318,130,321];[327,105,152,318,130,321];[329,104,153,317,130,321];[329,104,153,317,130,321];[329,103,154,316,130,321];[330,103,154,316,130,321];[331,102,154,315,130,321];[331,101,154,315,130,321];[331,101,154,315,130,321];[331,101,154,315,130,321];[332,101,154,315,130,321];[332,101,154,315,130,321];[332,100,154,315,130,321];[332,100,154,315,130,321];[332,100,154,315,130,321];[333,100,154,315,130,321];[333,100,154,315,130,321]];
ball_diam = 11;

x_min = min(min(pos(:,[1 3 5])));
x_max = max(max(pos(:,[1 3 5])));
y_min = min(min(pos(:,[2 4 6])));
y_max = max(max(pos(:,[2 4 6])));

ScoreSheet = figure(1);

hold on

plot(pos(:,1),pos(:,2),'color',[0.631 0.075 0.004],'lineStyle',' -');
plot(pos(:,3),pos(:,4),'color',[0.929 0.761 0.016],'lineStyle',' -');
plot(pos(:,5),pos(:,6),'color',[0.157 0.157 0.235],'lineStyle',' -');
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
plot(pos(ball_touch_a,2*player_ball-1),pos(ball_touch_a,2*player_ball),'o','lineStyle','none','color',[0.000000,0.729412,1.000000]);
plot(pos(ball_touch_b,2*player_ball-1),pos(ball_touch_b,2*player_ball),'o','lineStyle','none','color',[0.000000,0.729412,1.000000]);

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
scoresheet_title = sprintf('Scores Sheet - F1 - %s %dh%dmin%ds',date,time_t(4:6));
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