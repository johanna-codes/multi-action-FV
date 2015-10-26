function acc = post_processing (person_est, person_real, L)
person_est2 = person_est ;
changes = [];
chang=1;


for l =1:2
    changes = [];
    chang=1;

for i=2:length(person_est)

    if  (  person_est2(i) == person_est2(i-1) )
    chang = chang+1;
    
    else
        %stem(i-1,6, 'k--')
        changes = [changes  chang];
        
        if (chang<L)
            i;
            a = [i-chang:i-1];
           
            if (i-chang-1) ==0
                person_est2(i-chang:i-1) =  person_est2(1);
                person_est2(i-chang:i-1)';
                
            else
            person_est2(i-chang:i-1) =  person_est2(i-chang-1);
            person_est2(i-chang:i-1)';
            end;
            %pause
        end
        chang =1;
    end
 
end
end

changes = [changes chang];
%figure
%plot (person_est2, 'k*');
%hold on
%plot (person_real, 'r*');
 acc = sum( (person_est2==person_real) )/length(person_est);
 acc = acc*100;
end


