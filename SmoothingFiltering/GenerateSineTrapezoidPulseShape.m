

pulseLevel=zeros(1,100);

for pulseIdx=1:20
    pulseLevel(pulseIdx)=int16(255*(1-cos((pulseIdx-1)*(pi/2)/20)));
end

for pulseIdx=81:100
    pulseLevel(pulseIdx)=int16(255*(1-cos((100-pulseIdx)*(pi/2)/20)));
end

pulseLevel(21:80)=255;

figure; plot(pulseLevel)

pulseWave=sprintf('%d, ',pulseLevel);

pulseFile= ['const int sounddata_length=100;' ...
            'const unsigned char sounddata_data[] PROGMEM = {'...
            pulseWave ...
            '};'];