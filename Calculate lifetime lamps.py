import csv
import datetime
import pandas as pd
done = 1
saveto = 1

start = int(input("1 = Save and calculate lifetime, 2 = Only calculate lifetime: "))
if start == 1:
   while saveto == 1:
       readwrite = 1
       filename = input("Testfilename (without .csv): ") + '.csv'
       df=pd.read_csv(filename)
       msMAX=df['ms'].max()
       
       while readwrite == 1:
           Lampfilename = 0  
           lampfile = int(input("Save to which lamp file (1t/m4): "))
           if lampfile == 1:
                Lampfilename = "Lamp 1.csv"
      
           elif lampfile == 2:
                Lampfilename = "Lamp 2.csv"
      
           elif lampfile == 3:
                Lampfilename = "Lamp 3.csv"
      
           elif lampfile == 4:
                Lampfilename = "Lamp 4.csv"

           with open(Lampfilename, 'a+', newline='') as file:
               writer = csv.writer(file)
               writer.writerow([msMAX])

           readwrite = int(input("1 = save to another lamp file, 2 = continue: "))

           if readwrite == 2:
               break

       saveto = int(input("1 = save another test time to lamp file, 2 = continue to calculation: "))
       if saveto == 2:
           start = 2
           break
if start == 2:
   while done == 1:
         lampnumber = 0
         count = 0
         totaaluren = 0
         
         lampnumber = int(input("Lamp number(1t/m4): "))
         if lampnumber == 1:
            lampfilename2 = "Lamp 1.csv"
      
         elif lampnumber == 2:
            lampfilename2 = "Lamp 2.csv"
      
         elif lampnumber == 3:
            lampfilename2 = "Lamp 3.csv"
      
         elif lampnumber == 4:
            lampfilename2 = "Lamp 4.csv"
      
         else:   
            print ("Wrong input!") 
      
         with open(lampfilename2) as csvDataFile:
            lamp=list(csv.reader(csvDataFile))
          
         def convert(seconds):
            min, sec = divmod(seconds, 60) 
            hour, min = divmod(min, 60) 
            return "%d:%02d:%02d" % (hour, min, sec)

         file = open(lampfilename2)
         totalrows = len(file.readlines())

         while count < totalrows:  
            totaaluren = totaaluren + int(float((lamp[count][0])))
            count += 1 

         n = totaaluren /1000
         print("Total on hours: ", convert(n),  " in hours:minutes:seconds")
         print ("Totaal test runs: ", totalrows)

         done = int(input("1 = another lamp, 2 = stop: "))

         if done == 2:
             break
      
   

      
