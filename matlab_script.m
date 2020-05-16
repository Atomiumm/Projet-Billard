clear;clc; close all

pos = [[286,287,296,99,136,142];[287,287,296,103,136,142];[287,287,292,152,136,143];[287,287,286,208,136,142];[288,287,280,266,136,142];[308,311,251,302,136,143];[330,336,224,330,136,143];[353,362,192,365,136,142];[371,382,167,389,136,142];[386,381,135,359,136,143];[398,365,144,330,136,143];[408,352,167,305,136,142];[420,340,192,277,136,143];[431,327,216,250,136,142];[442,315,237,226,136,143];[453,301,258,202,136,142];[465,288,278,180,136,142];[478,275,298,157,136,143];[489,262,316,136,136,142];[499,251,334,116,136,142];[511,237,352,99,136,142];[523,223,376,117,136,142];[535,212,396,132,136,142];[547,197,418,145,136,142];[558,184,437,156,136,142];[570,170,460,170,136,142];[581,159,480,181,136,143];[594,144,504,195,136,143];[607,130,526,209,136,143];[619,116,548,222,136,143];[631,102,572,236,136,143];[642,99,594,249,136,143];[650,109,618,263,136,143];[658,115,640,276,136,143];[665,121,664,289,136,143];[673,127,686,303,136,143];[681,133,709,316,136,143];[689,139,732,329,136,143];[697,146,720,344,136,143];[705,152,705,359,136,143];[712,158,692,375,136,143];[720,164,679,390,136,143];[728,170,665,383,136,143];[733,175,652,374,136,143];[729,179,639,366,136,143];[725,183,626,357,136,143];[721,187,613,348,136,143];[718,191,600,340,136,143];[714,194,587,332,136,143];[710,198,575,324,136,143];[707,201,562,315,136,143];[704,205,550,307,136,143];[700,208,538,299,136,143];[697,211,526,292,136,143];[695,214,514,283,136,143];[691,218,502,275,136,143];[688,221,490,268,136,143];[685,224,478,260,136,143];[682,227,467,253,136,143];[679,230,456,245,136,143];[677,233,444,238,136,143];[674,235,432,230,136,143];[671,238,422,223,136,143];[668,241,410,215,136,143];[666,244,400,208,136,143];[663,246,388,201,136,143];[661,249,377,194,136,143];[659,251,366,187,136,143];[656,253,356,180,136,143];[654,255,345,173,136,143];[652,258,335,167,136,143];[650,259,324,160,136,143];[648,262,314,153,136,143];[646,264,304,147,136,143];[644,266,294,140,136,143];[642,267,284,134,136,143];[640,269,274,128,136,143];[639,271,264,121,136,143];[637,273,254,115,136,143];[635,274,244,109,136,143];[634,276,234,103,136,143];[632,277,225,99,136,143];[631,278,218,102,136,143];[630,280,210,105,136,143];[628,281,204,108,136,143];[627,282,196,111,136,143];[626,283,189,113,136,143];[625,285,182,116,136,143];[624,286,175,119,136,143];[623,287,168,122,136,143];[622,287,162,124,136,143];[621,288,154,127,136,143];[620,289,148,129,136,143];[620,289,142,131,135,144];[619,290,139,130,134,146];[619,290,136,129,132,147];[619,290,134,128,131,149];[618,291,132,128,130,151];[618,291,130,128,129,153];[618,291,130,127,130,153]];
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

plot(pos(band_touch,2*player_ball-1),pos(band_touch,2*player_ball),'sq','lineStyle','none','color',[1.000000,0.000000,0.768627]);
plot(pos(ball_touch_a,2*player_ball-1),pos(ball_touch_a,2*player_ball),'o','lineStyle','none','color',[0.203922,1.000000,0.164706]);
plot(pos(ball_touch_b,2*player_ball-1),pos(ball_touch_b,2*player_ball),'o','lineStyle','none','color',[0.203922,1.000000,0.164706]);

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
scoresheet_title = sprintf('Scores Sheet - F3 - %s %dh%dmin%ds',date,time_t(4:6));
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