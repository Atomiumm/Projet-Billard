clear;clc; close all

pos = [[321,102,442,228,500,173];[321,102,442,228,500,173];[321,102,442,228,463,156];[321,102,442,228,419,137];[321,102,442,228,379,119];[321,102,442,228,339,101];[292,107,442,228,324,112];[263,112,442,228,312,128];[237,117,442,228,297,143];[214,121,442,228,280,157];[192,125,442,228,262,171];[169,130,442,228,245,185];[147,134,442,228,229,198];[130,137,442,228,214,210];[147,137,442,228,198,223];[162,140,442,228,182,236];[174,141,442,228,166,249];[187,142,442,228,148,263];[199,143,442,228,133,275];[211,145,442,228,135,288];[223,146,442,228,145,301];[235,148,442,228,153,312];[247,149,442,228,162,325];[259,151,442,228,171,336];[271,152,442,228,180,348];[282,153,442,228,188,359];[293,155,442,228,197,371];[304,157,442,228,206,383];[317,157,442,228,215,389];[328,159,442,228,224,382];[338,160,442,228,233,375];[349,162,442,228,243,368];[360,163,442,228,251,362];[371,165,442,228,261,355];[382,166,442,228,269,349];[392,167,442,228,278,342];[402,169,442,228,286,336];[413,170,442,228,295,330];[423,171,442,228,303,324];[434,173,442,228,312,317];[443,174,442,228,320,311];[453,175,442,228,328,305];[463,176,442,228,335,299];[473,178,442,228,343,294];[483,179,442,228,351,288];[491,180,442,228,359,282];[501,181,442,228,366,276];[511,183,442,228,374,271];[520,184,442,228,381,265];[530,185,442,228,389,260];[538,186,442,228,396,254];[547,187,442,228,403,249];[556,188,442,228,411,243];[566,189,442,228,418,238];[575,190,442,228,424,233];[583,192,444,228,428,228];[591,193,450,228,429,224];[600,194,454,228,430,219];[609,195,458,227,430,215];[617,196,462,227,431,211];[625,197,466,227,432,207];[634,198,470,227,432,203];[643,199,474,227,432,199];[651,200,478,227,433,195];[659,201,482,227,434,191];[666,202,485,227,434,188];[674,203,488,227,435,184];[682,204,492,227,435,181];[690,205,496,227,436,177];[698,206,498,227,436,174];[704,207,500,227,437,171];[711,208,504,226,437,168];[719,209,506,226,438,165];[726,210,509,226,438,162];[732,211,512,226,438,159];[732,212,514,226,439,156];[728,213,516,226,439,154];[724,213,518,226,439,152];[720,213,520,226,439,149];[716,214,522,226,440,147];[713,215,524,226,440,144];[709,215,526,226,441,142];[706,217,528,226,441,140];[703,217,530,226,441,138];[699,217,532,226,441,136];[696,218,534,226,442,134];[693,218,534,226,442,133];[690,219,536,226,443,131];[687,219,538,227,443,129];[684,220,540,226,443,128];[682,220,540,227,444,126];[679,220,542,227,444,125];[677,221,543,227,444,124];[675,221,544,227,444,123];[672,221,544,227,445,122];[670,221,546,227,445,121];[668,221,546,227,445,120];[666,222,546,227,445,119];[664,222,546,227,445,118];[661,222,547,227,445,118];[661,223,547,227,446,117];[659,222,547,227,446,117];[657,222,547,227,446,117];[655,223,547,227,446,116]];
ball_diam = 11;

x_min = min(min(pos(:,[1 3 5])));
x_max = max(max(pos(:,[1 3 5])));
y_min = min(min(pos(:,[2 4 6])));
y_max = max(max(pos(:,[2 4 6])));

ScoreSheet = figure(1);

hold on

plot(pos(:,1),pos(:,2),'color',[0.811765 0.313725 0.313725],'lineStyle','--');
plot(pos(:,3),pos(:,4),'color',[0.772549 0.772549 0.274510],'lineStyle','--');
plot(pos(:,5),pos(:,6),'color',[0.000000 0.000000 0.000000],'lineStyle','--');
axis ij

line([x_min x_max],[y_min y_min]);
line([x_min x_max],[y_max y_max]);
line([x_min x_min],[y_min y_max]);
line([x_max x_max],[y_min y_max]);

posleft = [pos;pos(end,:);pos(end,:)];
posright = [pos(1,:);pos(1,:);pos];
pos = [pos(1,:);pos;pos(end,:)];

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

[player_ball,~] = find(moving',1);

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

plot(pos(band_touch,2*player_ball-1),pos(band_touch,2*player_ball),'Marker','*','lineStyle','none','color',[0.000000 0.000000 0.000000]);
plot(pos(ball_touch_a,2*player_ball-1),pos(ball_touch_a,2*player_ball),'Marker','o','lineStyle','none','color',[0.000000 0.000000 0.000000]);
plot(pos(ball_touch_b,2*player_ball-1),pos(ball_touch_b,2*player_ball),'Marker','o','lineStyle','none','color',[0.000000 0.000000 0.000000]);

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
scoresheet_title = sprintf('Scores Sheet - F6 - %s %dh%dmin%ds',date,time_t(4:6));
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