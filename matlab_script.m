clear;clc; close all

pos = [[321,102,444,121,498,352];[321,102,457,178,498,353];[321,102,457,178,498,353];[321,102,488,344,504,364];[321,102,468,358,535,364];[321,102,450,374,566,299];[321,102,433,390,592,245];[321,102,409,379,614,200];[321,102,386,369,636,156];[321,102,363,361,658,113];[321,102,341,353,671,108];[321,102,318,346,678,139];[321,102,296,338,685,165];[321,102,274,331,692,189];[321,102,252,323,702,211];[321,102,230,316,710,231];[321,102,208,309,718,251];[321,102,186,301,727,273];[321,102,164,294,734,292];[321,102,143,287,731,308];[321,102,130,278,727,325];[321,102,148,262,723,342];[321,102,162,247,720,359];[321,102,172,232,716,376];[321,102,182,218,712,392];[321,102,192,203,706,382];[321,102,202,190,701,372];[321,102,213,175,696,363];[321,102,222,162,691,353];[321,102,232,148,686,344];[321,102,242,134,681,335];[321,102,252,120,676,326];[321,102,262,107,672,317];[321,102,272,100,667,308];[321,102,286,109,662,300];[321,102,298,115,657,291];[321,102,312,121,653,283];[321,102,325,128,648,274];[321,103,338,134,643,266];[321,103,350,140,639,258];[321,103,363,146,635,249];[321,102,376,152,631,241];[321,102,388,158,626,234];[321,102,401,164,622,226];[321,102,414,170,618,218];[321,102,426,176,614,210];[321,102,438,181,609,203];[321,102,450,187,606,195];[321,102,462,193,601,188];[321,102,474,199,598,180];[321,102,486,205,594,173];[321,102,498,211,590,166];[321,102,510,216,586,159];[321,102,522,222,583,152];[321,102,533,227,579,145];[321,102,544,233,575,139];[321,102,556,238,572,132];[321,102,568,244,568,125];[321,102,579,250,564,119];[321,102,590,255,561,112];[321,102,602,261,557,106];[321,102,612,266,554,100];[321,102,624,271,551,98];[321,102,634,277,549,101];[321,102,644,282,547,105];[321,102,655,287,545,108];[321,102,666,292,543,111];[321,102,676,298,541,114];[321,102,687,303,539,117];[321,102,697,308,538,120];[321,102,708,313,536,123];[321,102,718,318,534,126];[321,102,728,323,532,128];[321,102,734,329,530,131];[321,102,728,337,529,133];[321,102,724,345,527,136];[321,102,718,353,526,138];[321,102,714,361,525,140];[321,102,709,369,523,143];[321,102,704,376,522,145];[321,102,699,384,520,147];[321,102,695,391,519,149];[321,102,689,390,518,150];[321,102,682,386,517,152];[321,102,676,382,515,154];[321,102,670,378,514,156];[321,102,664,374,513,157];[321,102,658,371,512,159];[321,102,651,367,511,160];[321,102,645,363,510,161];[321,102,640,360,509,163];[321,102,634,357,508,164];[321,102,628,353,507,165];[321,102,623,349,507,166];[321,102,618,346,506,167];[321,102,612,343,505,168];[321,102,607,340,504,168];[321,102,602,336,504,169];[321,102,597,333,503,170];[321,102,592,330,503,171];[321,102,588,327,502,171];[321,102,582,324,502,172];[321,102,578,321,501,172];[321,102,574,317,501,172];[321,102,568,314,501,173];[321,102,564,312,500,173];[321,102,560,309,500,173];[321,102,556,306,500,173];[321,102,552,303,500,173];[321,102,548,301,500,173];[321,102,544,298,500,173];[321,102,540,295,500,173];[321,102,536,293,500,173];[321,102,532,290,500,173];[321,102,528,288,500,173];[321,102,524,286,500,173];[321,102,522,283,500,173];[321,102,518,281,500,173];[321,102,514,279,500,173];[321,102,512,276,500,173];[321,102,508,274,500,173];[321,102,506,272,500,173];[321,102,502,270,500,173];[321,102,500,268,500,173];[321,102,496,266,500,173];[321,102,494,264,500,173];[321,102,491,262,500,173];[321,102,488,261,500,173];[321,102,486,259,500,173];[321,102,483,257,500,173];[321,102,480,255,500,173];[321,102,478,254,500,173];[321,102,476,252,500,173];[321,102,474,251,500,173];[321,102,472,250,500,173];[321,102,470,248,500,173];[321,102,468,246,500,173];[321,102,466,245,500,173];[321,102,464,244,500,173];[321,102,462,242,500,173];[321,102,461,241,500,173];[321,102,460,240,500,173];[321,102,458,239,500,173];[321,102,456,238,500,173];[321,102,455,237,500,173];[321,102,454,235,500,173];[321,102,452,235,500,173];[321,102,452,234,500,173];[321,102,450,233,500,173];[321,102,450,232,500,173];[321,102,448,232,500,173];[321,102,448,231,500,173];[321,102,446,231,500,173];[321,102,446,230,500,173];[321,102,446,229,500,173];[321,102,444,229,500,173];[321,102,444,229,500,173];[321,102,444,228,500,173];[321,102,444,228,500,173];[321,102,444,228,500,173]];
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

plot(pos(band_touch,2*player_ball-1),pos(band_touch,2*player_ball),'sq','lineStyle','none','color',[0.937255,0.000000,0.388235]);
plot(pos(ball_touch_a,2*player_ball-1),pos(ball_touch_a,2*player_ball),'o','lineStyle','none','color',[0.392157,1.000000,0.000000]);
plot(pos(ball_touch_b,2*player_ball-1),pos(ball_touch_b,2*player_ball),'o','lineStyle','none','color',[0.392157,1.000000,0.000000]);

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
scoresheet_title = sprintf('Scores Sheet - F5 - %s %dh%dmin%ds',date,time_t(4:6));
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