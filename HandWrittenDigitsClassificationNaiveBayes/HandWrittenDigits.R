fileName="trainingimages"
conn = file(fileName,open = "r")
linn = readLines(conn)
close(conn)
length(linn)

for(i in 1:5000){
  vec <- vector()
  test_vec <- vector()
  print(i)

  for(j in 1:28){
    x <- linn[(i-1)*28+j]
    x <- substring(x, seq(1,nchar(x),1), seq(1,nchar(x),1))
    x <- as.vector(x)
    vec <- c(vec,x)
    
    
    for(k in 1:28){
      #print(k)
      if(x[k] == "+"){
        test_vec <- c(test_vec,1)
      }else if(x[k] == "#"){
        test_vec<- c(test_vec,1)
      }else{
        test_vec <- c(test_vec,0)
      }
    }
  }
  
  
  #print(length(vec))
  if(i == 1){
    myMatrix <- matrix(test_vec,nrow = 1, ncol  =28*28) 
    #print(myMatrix)
  }else{
    myMatrix <- rbind(myMatrix,test_vec)
  }
}


fileName="traininglabels"

conn = file(fileName,open = "r")
linn2 = readLines(conn)
close(conn)
result_vector <- as.vector(linn2)
result_vector <- as.numeric(result_vector)
myMatrix <- cbind(myMatrix, result_vector)


library(e1071) 
data_frame_myMatrix <- as.data.frame(myMatrix)
rownames(data_frame_myMatrix) <- c(1:5000)
classifier<-naiveBayes((as.factor(result_vector)) ~ ., data = data_frame_myMatrix,laplace = 1) 

fileName="testimages"
conn = file(fileName,open = "r")
linn = readLines(conn)
close(conn)
length(linn)

for(i in 1:1000){
  vec <- vector()
  test_vec <- vector()
  print(i)
  for(j in 1:28){
    x <- linn[(i-1)*28+j]
    x <- substring(x, seq(1,nchar(x),1), seq(1,nchar(x),1))
    x <- as.vector(x)
    vec <- c(vec,x)
    
    for(k in 1:28){
      #print(k)
      if(x[k] == "+"){
        test_vec <- c(test_vec,1)
      }else if(x[k] == "#"){
        test_vec<- c(test_vec,1)
      }else{
        test_vec <- c(test_vec,0)
      }
    }
  }
  #print(length(vec))
  if(i == 1){
    myMatrix_testing_data <- matrix(test_vec,nrow = 1, ncol  =28*28) 
    #print(myMatrix)
  }else{
    myMatrix_testing_data <- rbind(myMatrix_testing_data,test_vec)
  }
}


fileName="testlabels"

conn = file(fileName,open = "r")
linn2 = readLines(conn)
close(conn)
result_vector <- as.vector(linn2)
result_vector <- as.numeric(result_vector)
myMatrix_testing_data <- cbind(myMatrix_testing_data, result_vector)


data_frame_myMatrix_testing_data <- as.data.frame(myMatrix_testing_data)
rownames(data_frame_myMatrix_testing_data) <- c(1:1000)

prediction <- predict(classifier, data_frame_myMatrix_testing_data[1:1000,-785])

tab <- table(pred = prediction, true = data_frame_myMatrix_testing_data[,785])

accu_sum <- 0
for(i in 1:10){
  #print(tab[i,i])
  accu_sum <- accu_sum+tab[i,i]
}
print(accu_sum/10)
data_frame_prediction <- as.data.frame(prediction)
write.table(data_frame_prediction, file = "testlabels_2.txt", append = FALSE,
            eol = "\n", na = "NA",row.names = TRUE,quote = FALSE,
            col.names = FALSE)


