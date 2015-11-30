% Assume vk are in the decreasing order
function [retransMatrix, retransMatrixTotalLen] = getInfoMatrix(stringDir)

fidin = fopen(stringDir,'r');
nline = 0;
while ~feof(fidin)
    tline = fgetl(fidin);
    nline = nline + 1;
end
retransMatrixTotalLen = nline;    
fclose(fidin);    
    
retransMatrix = zeros(1,retransMatrixTotalLen);
retransMatrixLen = 1;    
fidin = fopen(stringDir,'r');
while ~feof(fidin)
    %tline =  fgetl(fidin);
    if (retransMatrixLen == (retransMatrixTotalLen+1))
        break;
    end
    retransMatrix(retransMatrixLen) = (fscanf(fidin,'%f',1));
    retransMatrixLen = retransMatrixLen + 1;
end
%retransMatrixTotalLen = retransMatrixLen - 1;
fclose(fidin);
end