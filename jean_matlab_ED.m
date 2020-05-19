%% initialisation

clear;clc; close all

pos = %s
ball_diam = %d;


%% detection of billard boundaries

x_min = min(min(pos(:,[1 3 5])));
x_max = max(max(pos(:,[1 3 5])));
y_min = min(min(pos(:,[2 4 6])));
y_max = max(max(pos(:,[2 4 6])));

%% correct the positions

DistLimit = sqrt((x_max-x_min)^2+(y_max-y_min)^2)/2;
flag = 1;
while flag == 1
    flag = 0;
    speed = diff(pos);
    SqrtSpeed = [sqrt(speed(:,1).^2+speed(:,2).^2), sqrt(speed(:,3).^2+speed(:,4).^2), sqrt(speed(:,5).^2+speed(:,6).^2)];
    [row] =  find([0;SqrtSpeed(:,1)]>DistLimit,1);
    if size(row,1) > 0
        pos(row,1) = (pos(row-1,1)+pos(row+1,1))/2;
        pos(row,2) = (pos(row-1,2)+pos(row+1,2))/2;
        flag = 1;
    end
    [row] =  find([0;SqrtSpeed(:,2)]>DistLimit,1);
    if size(row,1) > 0
        pos(row,3) = (pos(row-1,3)+pos(row+1,3))/2;
        pos(row,4) = (pos(row-1,4)+pos(row+1,4))/2;
        flag = 1;
    end
    [row] =  find([0;SqrtSpeed(:,3)]>DistLimit,1);
    if size(row,1) > 0
        pos(row,5) = (pos(row-1,5)+pos(row+1,5))/2;
        pos(row,6) = (pos(row-1,6)+pos(row+1,6))/2;
        flag = 1;
    end
end


%% plot of the trajectories

ScoreSheet = figure(1);

hold on

plot(pos(:,1),pos(:,2),'color',[%f %f %f],'lineStyle','%s');
plot(pos(:,3),pos(:,4),'color',[%f %f %f],'lineStyle','%s');
plot(pos(:,5),pos(:,6),'color',[%f %f %f],'lineStyle','%s');
axis ij

line([x_min x_max],[y_min y_min]);
line([x_min x_max],[y_max y_max]);
line([x_min x_min],[y_min y_max]);
line([x_max x_max],[y_min y_max]);

%% detection of the first moving ball to detect player ball, and creation of arrays of interest
speed = diff(pos);
SqrtSpeed = [sqrt(speed(:,1).^2+speed(:,2).^2), sqrt(speed(:,3).^2+speed(:,4).^2), sqrt(speed(:,5).^2+speed(:,6).^2)];
acc = diff(speed);
SqrtAcc = [sqrt(acc(:,1).^2+acc(:,2).^2), sqrt(acc(:,3).^2+acc(:,4).^2), sqrt(acc(:,5).^2+acc(:,6).^2)];
SpeedLimit = max(max(SqrtSpeed))/10;
[FirstBall,row] = find(SqrtSpeed'>SpeedLimit,1);


%% Find points of interest
SqrtAcc = [sqrt(acc(:,1).^2+acc(:,2).^2), sqrt(acc(:,3).^2+acc(:,4).^2), sqrt(acc(:,5).^2+acc(:,6).^2)];
AccLimit = ceil(sqrt(max(SqrtAcc)));

TouchBallRed = 0*pos(:,1);
TouchBallYellow = 0*pos(:,1);
TouchBallWhite = 0*pos(:,1);
TouchBandLeft = [0;SqrtAcc(:,FirstBall)>AccLimit(FirstBall);0] & abs(pos(:,2*FirstBall-1)-x_min) < ball_diam;
TouchBandRight = [0;SqrtAcc(:,FirstBall)>AccLimit(FirstBall);0] & abs(pos(:,2*FirstBall-1)-x_max) < ball_diam;
TouchBandBottom = [0;SqrtAcc(:,FirstBall)>AccLimit(FirstBall);0] & abs(pos(:,2*FirstBall)-y_min) < ball_diam;
TouchBandTop = [0;SqrtAcc(:,FirstBall)>AccLimit(FirstBall);0] & abs(pos(:,2*FirstBall)-y_max) < ball_diam;
if FirstBall == 1
    TouchBallYellow = [0;SqrtAcc(:,FirstBall)>sqrt(AccLimit(FirstBall));0] & sqrt((pos(:,1)-pos(:,3)).^2+(pos(:,2)-pos(:,4)).^2) < 5*ball_diam;
    TouchBallWhite = [0;SqrtAcc(:,FirstBall)>sqrt(AccLimit(FirstBall));0] & sqrt((pos(:,5)-pos(:,1)).^2+(pos(:,6)-pos(:,2)).^2) < 5*ball_diam;
elseif FirstBall == 2
    TouchBallRed = [0;SqrtAcc(:,FirstBall)>sqrt(AccLimit(FirstBall));0] & sqrt((pos(:,1)-pos(:,3)).^2+(pos(:,2)-pos(:,4)).^2) < 5*ball_diam;
    TouchBallWhite = [0;SqrtAcc(:,FirstBall)>sqrt(AccLimit(FirstBall));0] & sqrt((pos(:,5)-pos(:,3)).^2+(pos(:,6)-pos(:,4)).^2) < 5*ball_diam;
elseif FirstBall == 3
    TouchBallRed = [0;SqrtAcc(:,FirstBall)>sqrt(AccLimit(FirstBall));0] & sqrt((pos(:,1)-pos(:,5)).^2+(pos(:,2)-pos(:,6)).^2) < 5*ball_diam;
    TouchBallYellow = [0;SqrtAcc(:,FirstBall)>sqrt(AccLimit(FirstBall));0] & sqrt((pos(:,5)-pos(:,3)).^2+(pos(:,6)-pos(:,4)).^2) < 5*ball_diam;
end


%% Clean up uninteresting points

[FirstBallTouchRow, FirstBallTouch] = min([find(TouchBallRed,1); find(TouchBallYellow,1); find(TouchBallWhite,1)]);
TouchBandLeft(1:FirstBallTouchRow-1) = 0*TouchBandLeft(1:FirstBallTouchRow-1);
TouchBandRight(1:FirstBallTouchRow-1) = 0*TouchBandRight(1:FirstBallTouchRow-1);
TouchBandBottom(1:FirstBallTouchRow-1) = 0*TouchBandBottom(1:FirstBallTouchRow-1);
TouchBandTop(1:FirstBallTouchRow-1) = 0*TouchBandTop(1:FirstBallTouchRow-1);

if (FirstBall == 1 & FirstBallTouch == 2) | (FirstBall == 2 & FirstBallTouch == 1)
    SecondBallTouch = 3;
    SecondBallTouchRow = find(TouchBallWhite,1);
elseif (FirstBall == 2 & FirstBallTouch == 3) | (FirstBall == 3 & FirstBallTouch == 2)
    SecondBallTouch = 1;
    SecondBallTouchRow = find(TouchBallRed,1);
elseif (FirstBall == 1 & FirstBallTouch == 3) | (FirstBall == 3 & FirstBallTouch == 1)
    SecondBallTouch = 2;
    SecondBallTouchRow = find(TouchBallYellow,1);
end

if size(SecondBallTouchRow,1) > 0
    TouchBandLeft(SecondBallTouchRow+1:end) = 0*TouchBandLeft(SecondBallTouchRow+1:end);
    TouchBandRight(SecondBallTouchRow+1:end) = 0*TouchBandRight(SecondBallTouchRow+1:end);
    TouchBandBottom(SecondBallTouchRow+1:end) = 0*TouchBandBottom(SecondBallTouchRow+1:end);
    TouchBandTop(SecondBallTouchRow+1:end) = 0*TouchBandTop(SecondBallTouchRow+1:end);
    TouchBallRed(SecondBallTouchRow+1:end) = 0*TouchBallRed(SecondBallTouchRow+1:end);
    TouchBallYellow(SecondBallTouchRow+1:end) = 0*TouchBallYellow(SecondBallTouchRow+1:end);
    TouchBallWhite(SecondBallTouchRow+1:end) = 0*TouchBallWhite(SecondBallTouchRow+1:end);
end

%% Plot interesting points

plot(pos(TouchBandLeft, FirstBall*2-1), pos(TouchBandLeft, FirstBall*2), 'color',[%f %f %f],'Marker','%s','linestyle','none')
plot(pos(TouchBandRight, FirstBall*2-1), pos(TouchBandRight, FirstBall*2), 'color',[%f %f %f],'Marker','%s','linestyle','none')
plot(pos(TouchBandBottom, FirstBall*2-1), pos(TouchBandBottom, FirstBall*2), 'color',[%f %f %f],'Marker','%s','linestyle','none')
plot(pos(TouchBandTop, FirstBall*2-1), pos(TouchBandTop, FirstBall*2), 'color',[%f %f %f],'Marker','%s','linestyle','none')
if find(TouchBallRed,1) 
    plot(pos(TouchBallRed, FirstBall*2-1), pos(TouchBallRed, FirstBall*2), 'color',[%f %f %f],'Marker','%s','linestyle','none') 
end
if find(TouchBallYellow,1) 
    plot(pos(TouchBallYellow, FirstBall*2-1), pos(TouchBallYellow, FirstBall*2), 'color',[%f %f %f],'Marker','%s','linestyle','none') 
end
if find(TouchBallWhite,1) 
    plot(pos(TouchBallWhite, FirstBall*2-1), pos(TouchBallWhite, FirstBall*2), 'color',[%f %f %f],'Marker','%s','linestyle','none') 
end


%% Get winning information

dist(1) = floor(sum(sqrt(speed(:,1).^2+speed(:,2).^2)));
dist(2) = floor(sum(sqrt(speed(:,3).^2+speed(:,4).^2)));
dist(3) = floor(sum(sqrt(speed(:,5).^2+speed(:,6).^2)));

if size(SecondBallTouchRow,1) > 0 && size(FirstBallTouchRow,1) > 0
    TouchBallsMess = "2 balls touched";
elseif size(SecondBallTouchRow,1) > 0
    TouchBallsMess = "1 ball touched";
else
    TouchBallsMess = "No ball touched";
end

TouchBands = sum([sum(TouchBandLeft) sum(TouchBandRight) sum(TouchBandBottom) sum(TouchBandTop)]);
if sum(TouchBands) >= 2
    TouchBandsMess = sprintf("\%d bands touched",sum(TouchBands));
elseif sum(TouchBands) == 1
    TouchBandsMess = "1 band touched";
else
    TouchBandsMess = "No band touched";
end

WinMess = "You lost. You litte shit.";
if size(SecondBallTouchRow,1) > 0 & TouchBands > 2
    WinMess = "You won. You're wonderful <3"; 
end

if FirstBall == 1
    Player = "Red";
elseif FirstBall == 2
    Player = "Yellow";
elseif FirstBall == 3
    Player = "White";
end




time_t = floor(clock);
scoresheet_title = sprintf('Score Sheet %s - \%s - \%d h \%d min \%d s',date,time_t(4:6));
sgtitle(scoresheet_title)

text_to_display = {sprintf("Scores for player \%s",Player),TouchBallsMess,TouchBandsMess,sprintf("dist(r) = \%dpx",dist(1)),sprintf("dist(y) = \%dpx",dist(2)),sprintf("dist(w) = \%dpx",dist(3)),WinMess};
x_text = x_min + [1 13 13 1 7 13 1]*(x_max-x_min)/18;
y_text = y_max + [14 14 43 72 72 72 43];
text(x_text,y_text,text_to_display);

axis([x_min x_max y_min-5 y_max+100]);
set(gca,'visible','off');
set(gca,'xtick',[]);

set(gcf, 'PaperUnits', 'centimeters', 'Units', 'centimeters');
set(gcf, 'PaperSize', [29.7 21]);
set(gcf, 'Position', [0.5 0.5 28.7 20]);

saveas(ScoreSheet,'ScoreSheet.pdf');
open('ScoreSheet.pdf');