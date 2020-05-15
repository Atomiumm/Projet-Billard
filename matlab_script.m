clear;clc; close all

pos = [[321,102,442,228,500,173];[321,102,442,228,500,173];[321,102,442,228,463,156];[321,102,442,228,412,134];[321,102,442,228,379,119];[321,102,442,228,336,100];[292,107,442,228,324,112];[263,112,442,228,312,128];[237,117,442,228,297,143];[214,121,442,228,280,157];[192,125,442,228,262,171];[169,129,442,228,245,185];[147,133,442,228,229,198];[130,137,442,228,214,210];[147,137,442,228,198,223];[162,139,442,228,182,236];[174,141,442,228,166,249];[187,142,442,228,148,263];[200,143,442,228,133,275];[212,145,442,228,134,288];[223,146,442,228,144,300];[235,148,442,228,153,312];[247,149,442,228,161,324];[259,151,442,228,171,336];[271,152,442,228,180,347];[282,153,442,228,188,359];[293,155,442,228,197,371];[305,157,442,228,206,383];[317,157,442,228,215,389];[328,159,442,228,224,382];[338,160,442,228,233,375];[349,162,442,228,243,368];[360,163,442,228,251,362];[371,165,442,228,261,355];[382,166,442,228,269,349];[392,167,442,228,278,342];[402,169,442,228,286,336];[413,170,442,228,295,330];[423,171,442,228,303,324];[434,173,442,228,312,317];[443,174,442,228,320,311];[453,175,442,228,328,305];[463,176,442,228,335,299];[473,178,442,228,343,294];[483,179,442,228,351,288];[491,180,442,228,359,282];[501,181,442,228,366,276];[511,183,442,228,374,271];[520,184,442,228,381,265];[530,185,442,228,389,260];[538,186,442,228,396,254];[547,187,442,228,403,249];[556,188,442,228,411,243];[565,189,442,228,418,238];[575,190,442,228,424,233];[583,192,444,228,428,228];[591,193,450,228,429,223];[600,194,454,228,430,219];[609,195,458,227,430,215];[617,196,462,227,431,211];[625,197,466,227,432,207];[634,198,470,227,432,203];[642,199,474,227,432,199];[651,200,478,227,433,195];[659,201,482,227,434,191];[666,202,485,227,434,188];[674,203,488,227,435,184];[681,204,492,227,435,181];[690,205,496,227,436,177];[697,206,498,227,437,174];[704,207,500,227,437,171];[711,208,504,227,437,168];[718,209,506,227,438,165];[726,210,509,226,438,162];[732,211,512,227,438,159];[732,212,514,226,439,156];[728,213,516,226,439,154];[724,213,518,226,439,151];[720,213,520,226,439,149];[716,214,522,226,440,147];[712,215,524,226,440,144];[709,215,526,226,441,142];[705,216,528,226,441,140];[702,217,530,226,441,138];[699,217,532,226,441,136];[696,217,534,226,442,134];[693,218,535,226,442,133];[690,219,536,226,443,131];[687,219,538,227,443,129];[684,220,540,227,443,128];[682,220,541,227,444,126];[679,221,542,227,444,125];[677,221,543,227,444,124];[674,222,544,227,444,123];[672,222,544,227,445,122];[670,222,546,227,445,121];[668,222,546,227,445,120];[666,222,546,227,445,119];[664,222,547,227,445,118];[661,222,547,227,445,118];[660,223,547,227,446,117];[658,223,547,227,446,117];[657,222,547,227,446,117];[654,223,547,227,446,116]];
ball_diam = 11;

x_min = min(min(pos(:,[1 3 5])));
x_max = max(max(pos(:,[1 3 5])));
y_min = min(min(pos(:,[2 4 6])));
y_max = max(max(pos(:,[2 4 6])));

ScoreSheet = figure(1);

hold on

plot(pos(:,1),pos(:,2),'r-',pos(:,3),pos(:,4),'y-',pos(:,5),pos(:,6),'k-');
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

velleft=[vel(2:end,:);vel(end,:)];
velright=[vel(1,:);vel(1:end-1,:)];

moving(:,1) = (pos(:,1)-pos(1,1)).^2 + (pos(:,2)-pos(1,2)).^2 >= 100;
moving(:,2) = (pos(:,3)-pos(1,3)).^2 + (pos(:,4)-pos(1,4)).^2 >= 100;
moving(:,3) = (pos(:,5)-pos(1,5)).^2 + (pos(:,6)-pos(1,6)).^2 >= 100;

moving_ind = find(moving);
player_ball = ceil(min(moving_ind(mod(moving_ind-1,n_frames)==min(mod(moving_ind-1,n_frames))))/n_frames);
fprintf("player ball : %d\n",player_ball);

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

plot(pos(band_touch,2*player_ball-1),pos(band_touch,2*player_ball),'*');
plot(pos(ball_touch_a,2*player_ball-1),pos(ball_touch_a,2*player_ball),'o');
plot(pos(ball_touch_b,2*player_ball-1),pos(ball_touch_b,2*player_ball),'o');

win = 0;

if numel(ball_touch_a)==0 || numel(ball_touch_b)==0 || numel(band_touch)<3
    fprintf("win : %d\n",win);
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

fprintf("win : %d\n",win);
saveas(ScoreSheet,'ScoreSheet','pdf');

return