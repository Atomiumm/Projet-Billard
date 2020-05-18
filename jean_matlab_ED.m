%% initialisation

clear;clc; close all

pos = [[287,287,296,99,136,142];[287,287,296,103,136,142];[287,287,292,152,136,143];[287,287,288,208,136,142];[287,287,280,266,137,143];[308,311,250,303,137,144];[330,336,224,330,136,143];[353,362,192,365,136,142];[371,382,166,388,136,142];[386,381,135,359,136,143];[397,365,144,330,136,142];[408,352,167,305,136,142];[419,339,192,277,136,143];[431,327,216,250,136,142];[442,315,238,225,136,143];[453,301,258,202,136,142];[465,288,278,180,136,142];[478,275,298,157,136,143];[489,262,316,136,136,142];[499,250,334,116,136,143];[510,237,352,99,136,142];[523,223,376,117,136,142];[534,211,396,132,136,143];[547,197,418,145,136,142];[558,184,437,156,136,142];[570,170,460,170,136,142];[581,159,480,182,136,143];[593,144,504,195,136,142];[607,130,526,209,136,143];[618,116,548,222,136,142];[631,102,572,236,136,143];[642,100,594,249,136,142];[650,109,616,262,136,143];[658,115,640,276,136,142];[665,121,664,289,136,143];[673,128,686,303,136,142];[681,134,709,316,136,143];[689,140,732,329,136,142];[697,146,721,344,136,143];[705,152,705,359,136,142];[712,158,692,375,136,143];[720,164,679,390,136,142];[728,170,666,384,136,143];[733,175,652,374,136,142];[729,180,639,366,136,143];[725,183,626,357,136,142];[721,187,613,349,136,143];[718,191,600,340,136,142];[714,194,588,332,136,143];[710,198,576,324,136,142];[707,201,562,315,136,143];[704,205,550,307,136,142];[701,208,538,299,136,143];[697,211,526,292,136,142];[695,214,514,283,136,143];[691,218,502,275,136,142];[688,221,490,268,136,143];[685,224,478,260,136,142];[682,227,467,253,136,143];[679,230,456,245,136,142];[677,233,444,238,136,143];[674,235,432,230,136,142];[671,239,422,223,136,143];[669,241,410,215,136,142];[666,244,399,208,136,143];[663,246,388,201,136,142];[661,249,377,194,136,143];[659,251,366,187,136,142];[656,253,356,181,136,143];[654,255,345,174,136,142];[652,258,335,167,136,143];[650,259,324,160,136,142];[648,262,314,154,136,143];[646,264,304,147,136,142];[644,266,294,140,136,143];[642,267,284,134,136,142];[641,269,274,128,136,143];[639,271,264,121,136,142];[637,273,254,115,136,143];[636,274,244,109,136,142];[634,276,234,103,136,143];[632,277,225,99,136,142];[631,279,218,102,136,143];[630,280,210,105,136,142];[629,281,204,108,136,143];[627,282,196,111,136,142];[626,283,189,113,136,143];[625,285,182,116,136,142];[624,286,176,119,136,143];[623,287,168,122,136,142];[622,287,162,124,136,143];[621,288,154,127,136,142];[620,289,148,129,136,143];[620,289,142,131,135,143];[619,290,139,130,134,146];[619,290,136,129,132,147];[619,290,134,128,131,149];[618,291,132,128,130,151];[618,291,130,128,130,153];[618,291,130,127,130,153]];
ball_diam = 11;

%pos : n*6 matrix containing (xred, yred, xyel, yyel, xwhite, ywhite) for
%each n valid frame

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

plot(pos(:,1),pos(:,2),'color',[1 0 0],'lineStyle','-');
plot(pos(:,3),pos(:,4),'color',[1 1 0],'lineStyle','-');
plot(pos(:,5),pos(:,6),'color',[0 0 0],'lineStyle','-');
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
[row,col] = find(SqrtSpeed'>SpeedLimit,1)

if col == 1
    FirstBall = "Red";
elseif col == 2
    FirstBall = "Yellow";
elseif col == 3
    FirstBall = "White";
end


%% Find points of interest
SqrtAcc = [sqrt(acc(:,1).^2+acc(:,2).^2), sqrt(acc(:,3).^2+acc(:,4).^2), sqrt(acc(:,5).^2+acc(:,6).^2)];
AccLimit = ceil(max(SqrtAcc)/15);


if FirstBall == "Red"
    RPointInterest = find(SqrtAcc(:,1)>AccLimit(1))+1;
    plot(pos(RPointInterest, 1), pos(RPointInterest, 2), "*g")
elseif FirstBall == "Yellow"
    YPointInterest = find(SqrtAcc(:,2)>AccLimit(2))+1;
    plot(pos(YPointInterest, 3), pos(YPointInterest, 4), "*g")
elseif FirstBall == "White"
    WPointInterest = find(SqrtAcc(:,3)>AccLimit(3))+1;
    plot(pos(WPointInterest, 5), pos(WPointInterest, 6), "*g")
end

if FirstBall == "Red"
    RPointInterest = abs(pos(:,1)-x_min)<ball_diam | abs(pos(:,1)-x_max)<ball_diam | abs(pos(:,2)-y_min)<ball_diam | abs(pos(:,2)-y_max)<ball_diam | sqrt((pos(:,1)-pos(:,3)).^2+(pos(:,2)-pos(:,4)).^2)<5*ball_diam | sqrt((pos(:,1)-pos(:,5)).^2+(pos(:,2)-pos(:,6)).^2)<5*ball_diam;
    plot(pos(RPointInterest, 1), pos(RPointInterest, 2), "*b")
elseif FirstBall == "Yellow"
    YPointInterest = abs(pos(:,3)-x_min)<ball_diam | abs(pos(:,3)-x_max)<ball_diam | abs(pos(:,4)-y_min)<ball_diam | abs(pos(:,4)-y_max)<ball_diam | sqrt((pos(:,1)-pos(:,3)).^2+(pos(:,2)-pos(:,4)).^2)<5*ball_diam | sqrt((pos(:,3)-pos(:,5)).^2+(pos(:,4)-pos(:,6)).^2)<5*ball_diam;
    plot(pos(YPointInterest, 3), pos(YPointInterest, 4), "*b")
elseif FirstBall == "White"
    WPointInterest = abs(pos(:,5)-x_min)<ball_diam | abs(pos(:,5)-x_max)<ball_diam | abs(pos(:,6)-y_min)<ball_diam | abs(pos(:,6)-y_max)<ball_diam | sqrt((pos(:,1)-pos(:,5)).^2+(pos(:,2)-pos(:,6)).^2)<5*ball_diam | sqrt((pos(:,3)-pos(:,5)).^2+(pos(:,4)-pos(:,6)).^2)<5*ball_diam;
    plot(pos(WPointInterest, 5), pos(WPointInterest, 6), "*b")
end
