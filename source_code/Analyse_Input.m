\%
\%	File:				Analysis_Input.m
\%
\%	Description: 		Analysis of succesive ball positions
\%	
\%	Full Description: 	Takes the list of succesive positions of the ball. 
\%						Detects the player ball. 
\%						Computes the collision with other balls and billard bands.
\%						Conclude on the sequence outcome. 
\%						Stores diagnosis in ScoreSheet.pdf
\%
\%	Dependancy:			none
\%
\%	Authors:			Dufour Edouard; Rochet Corentin		No copyright
\%
\%	Version:			3.2.7
\%
\%	Revision:			06.06.2020, ED, Updated outlier detection method
\%

clear;clc; close all

pos = %s;
ball_diam = %d;


\%\%find outliers
outliers(:,1) = (isoutlier(pos(:,1),'movmedian',5)&isoutlier(pos(:,2),'movmedian',5));
outliers(:,2) = (isoutlier(pos(:,1),'movmedian',5)&isoutlier(pos(:,2),'movmedian',5));
outliers(:,3) = (isoutlier(pos(:,3),'movmedian',5)&isoutlier(pos(:,4),'movmedian',5));
outliers(:,4) = (isoutlier(pos(:,3),'movmedian',5)&isoutlier(pos(:,4),'movmedian',5));
outliers(:,5) = (isoutlier(pos(:,5),'movmedian',5)&isoutlier(pos(:,6),'movmedian',5));
outliers(:,6) = (isoutlier(pos(:,5),'movmedian',5)&isoutlier(pos(:,6),'movmedian',5));
\%make a linear interpolation to solve the outliers
pos = pos.*~outliers + outliers.*([pos(1,:); pos(1:end-1,:)]+[pos(2:end,:); pos(end,:)])/2;
clearvars outliers


\%\%find border
x_min = min(min(pos(:,[1 3 5])));
x_max = max(max(pos(:,[1 3 5])));
y_min = min(min(pos(:,[2 4 6])));
y_max = max(max(pos(:,[2 4 6])));


\%\%plot data
ScoreSheet = figure(1);
hold on
plot(pos(:,1),pos(:,2),'color',[%d %d %d]/255,'lineStyle','%s','Marker','.');
plot(pos(:,3),pos(:,4),'color',[%d %d %d]/255,'lineStyle','%s','Marker','.');
plot(pos(:,5),pos(:,6),'color',[%d %d %d]/255,'lineStyle','%s','Marker','.');
axis ij

line([x_min x_max],[y_min y_min]);
line([x_min x_max],[y_max y_max]);
line([x_min x_min],[y_min y_max]);
line([x_max x_max],[y_min y_max]);


\%\%creation of arrays of interest
speed = diff(pos);


\%\%detect first ball
[FirstBall,FirstRow] = find(abs(pos-pos(1,:))'>2,1);
FirstBall = ceil(FirstBall/2);


\%\%detect band touch
SpeedDirectionChange = [0 0;diff(sign(speed(:,2*FirstBall-1:2*FirstBall)));0 0];
\%This is the maximum distance from the corresponding table border to be considered a collision
\%The distance condition depends on the speed for reasons of precision
DistanceCondition = (ball_diam*sqrt(abs([0 0;speed(:,2*FirstBall-1:2*FirstBall)])));
TouchBandLeft = (SpeedDirectionChange(:,1) > 0) & (abs(pos(:,2*FirstBall-1)-x_min) < DistanceCondition(:,1));
TouchBandRight = (SpeedDirectionChange(:,1) < 0) & (abs(pos(:,2*FirstBall-1)-x_max) < DistanceCondition(:,1));
TouchBandBottom = (SpeedDirectionChange(:,2) < 0) & (abs(pos(:,2*FirstBall)-y_max) < DistanceCondition(:,2));
TouchBandTop = (SpeedDirectionChange(:,2) > 0) & (abs(pos(:,2*FirstBall)-y_min) < DistanceCondition(:,2));
clearvars SpeedDirectionChange DistanceCondition;

\%\%detect ball touch
BallTouch = [];
\%caluclates the distance of the firstball compared to each of the balls
distance(:,1) = sqrt((pos(:,1)-pos(:,2*FirstBall-1)).^2+(pos(:,2)-pos(:,2*FirstBall)).^2);
distance(:,2) = sqrt((pos(:,3)-pos(:,2*FirstBall-1)).^2+(pos(:,4)-pos(:,2*FirstBall)).^2);
distance(:,3) = sqrt((pos(:,5)-pos(:,2*FirstBall-1)).^2+(pos(:,6)-pos(:,2*FirstBall)).^2);
\%This is the distance between balls to be considered a collision
\%The distance condition depends on the speed for reasons of precision
DistanceCondition = ball_diam*sqrt(1+abs([0;speed(:,2*FirstBall-1)]));
if FirstBall ~= 1 & find(sum(abs(pos(:,1:2)-pos(1,1:2))')'>4,1)
    \%detects if the ball moved
    moved = circshift(sum(abs(pos(:,1:2)-pos(1,1:2))')'>4,-3);
    \%temporary variable storing conditions multiplied with distance between the balls
    temp = moved.*distance(:,1).*(distance(:,1)<DistanceCondition);
    \%find the first spot respecting the conditions that is a local distance minimum
    BallTouch(length(BallTouch)+1) = find(islocalmin(temp+1e20*(temp==0)),1);
end
if FirstBall ~= 2 & find(sum(abs(pos(:,3:4)-pos(1,3:4))')'>4,1)
    moved = circshift(sum(abs(pos(:,3:4)-pos(1,3:4))')'>4,-3);
    temp = moved.*distance(:,2).*(distance(:,2)<DistanceCondition);
    BallTouch(length(BallTouch)+1) = find(islocalmin(temp+1e20*(temp==0)),1);
end
if FirstBall ~= 3 & find(sum(abs(pos(:,5:6)-pos(1,5:6))')'>4,1)
    moved = circshift(sum(abs(pos(:,5:6)-pos(1,5:6))')'>4,-3);
    temp = moved.*distance(:,3).*(distance(:,3)<DistanceCondition);
    BallTouch(length(BallTouch)+1) = find(islocalmin(temp+1e20*(temp==0)),1);
end
clearvars distance DistanceCondition moved temp;


\%\%clean up
\%deleting the table border collisions before the first ball collision
TouchBandLeft(1:min(BallTouch)) = zeros(size(TouchBandLeft(1:min(BallTouch))));
TouchBandRight(1:min(BallTouch)) = zeros(size(TouchBandRight(1:min(BallTouch))));
TouchBandBottom(1:min(BallTouch)) = zeros(size(TouchBandBottom(1:min(BallTouch))));
TouchBandTop(1:min(BallTouch)) = zeros(size(TouchBandTop(1:min(BallTouch))));
\%deleting the table border collisions after the second ball collision
if length(BallTouch) > 1
    TouchBandLeft(max(BallTouch):end) = zeros(size(TouchBandLeft(max(BallTouch):end)));
    TouchBandRight(max(BallTouch):end) = zeros(size(TouchBandRight(max(BallTouch):end)));
    TouchBandBottom(max(BallTouch):end) = zeros(size(TouchBandBottom(max(BallTouch):end)));
    TouchBandTop(max(BallTouch):end) = zeros(size(TouchBandTop(max(BallTouch):end)));
end


\%\%plot collisions
BandCollColor = [%d %d %d]/255;
plot(pos(TouchBandLeft, FirstBall*2-1), pos(TouchBandLeft, FirstBall*2), 'color',BandCollColor,'Marker','o','linestyle','none')
plot(pos(TouchBandRight, FirstBall*2-1), pos(TouchBandRight, FirstBall*2), 'color',BandCollColor,'Marker','o','linestyle','none')
plot(pos(TouchBandBottom, FirstBall*2-1), pos(TouchBandBottom, FirstBall*2), 'color',BandCollColor,'Marker','o','linestyle','none')
plot(pos(TouchBandTop, FirstBall*2-1), pos(TouchBandTop, FirstBall*2), 'color',BandCollColor,'Marker','o','linestyle','none')
plot(pos(BallTouch, FirstBall*2-1), pos(BallTouch, FirstBall*2), 'color',[%d %d %d]/255,'Marker','*','linestyle','none')


\%\%get messages
\%calculates the traveled distances
dist(1) = floor(sum(sqrt(speed(:,1).^2+speed(:,2).^2)));
dist(2) = floor(sum(sqrt(speed(:,3).^2+speed(:,4).^2)));
dist(3) = floor(sum(sqrt(speed(:,5).^2+speed(:,6).^2)));
\%find how many balls were touched
if length(BallTouch) > 1
    TouchBallsMess = "2 balls touched";
elseif length(BallTouch) > 0
    TouchBallsMess = "1 ball touched";
else
    TouchBallsMess = "No ball touched";
end
\%find how many table borders were touched
TouchBands = sum([sum(TouchBandLeft) sum(TouchBandRight) sum(TouchBandBottom) sum(TouchBandTop)]);
if sum(TouchBands) >= 2
    TouchBandsMess = sprintf("\%d bands touched",sum(TouchBands));
elseif sum(TouchBands) == 1
    TouchBandsMess = "1 band touched";
else
    TouchBandsMess = "No band touched";
end
\%get winning message
WinMess = "You lost.";
if length(BallTouch) > 1 && TouchBands > 2
    WinMess = "You won."; 
end
\%get first player ball
if FirstBall == 1
    Player = "Red";
elseif FirstBall == 2
    Player = "Yellow";
else
    Player = "White";
end


\%\%build title and text
time_t = floor(clock);
scoresheet_title = sprintf('Score Sheet %s - \%s - \%dh \%dmin \%ds',date,time_t(4:6));
sgtitle(scoresheet_title)

text_to_display = {sprintf("Scores for player \%s",Player),TouchBallsMess,TouchBandsMess,sprintf("dist(r) = \%dpx",dist(1)),sprintf("dist(y) = \%dpx",dist(2)),sprintf("dist(w) = \%dpx",dist(3)),WinMess};
x_text = x_min + [1 13 13 1 7 13 1]*(x_max-x_min)/18;
y_text = y_max + [14 14 43 72 72 72 43];
text(x_text,y_text,text_to_display);


\%\%create and save scoresheet
axis([x_min x_max y_min-5 y_max+100]);
set(gca,'visible','off');
set(gca,'xtick',[]);
set(gcf, 'PaperUnits', 'centimeters', 'Units', 'centimeters');
set(gcf, 'PaperSize', [29.7 21]);
set(gcf, 'Position', [0.5 0.5 28.7 20]);

saveas(ScoreSheet,'..\\ScoreSheet','pdf');
open('..\\ScoreSheet.pdf');