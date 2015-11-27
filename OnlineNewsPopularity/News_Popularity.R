Data <- read.csv(file="~/Desktop/OnlineNewsPopularity/OnlineNewsPopularity.csv", header=TRUE, sep=",")
# removing First two columns

Data <- Data[,c(-1,-2)]

# pre processing Data

Data$shares <- ifelse(Data$shares>1400,1,0)

indexes = sample(1:nrow(Data), size=0.3*nrow(Data))

testing_data <- Data[indexes,]
dim(testing_data)
testing_data$shares <- as.factor(testing_data$shares)

training_data <- Data[-indexes,]
dim(training_data)
training_data$shares <- as.factor(training_data$shares)




# DECISSION TREES

library(tree)
decision_tree <- tree(shares~. , training_data)

dt_predict <- predict(decision_tree, subset(testing_data, select = -shares), type = "class")

dt_predict_table<-table(predicted_condition = dt_predict, true_condition = testing_data$shares)

# Naive Bayes

library(e1071)
nb_classifier <- naiveBayes((as.factor(shares))~., data = training_data)
nb_predict <- predict(nb_classifier,subset(testing_data, select = -shares) )
nb_predict_table <- table(predicted_condition = nb_predict, true_condition = testing_data$shares)

# Logistic regression

log_reg_classifier <- glm((as.factor(shares))~. , data = training_data, family = binomial)
log_reg_predict_probs <- predict(log_reg_classifier, subset(testing_data, select = -shares),type = "response")
log_reg_predict <- rep(0,nrow(testing_data))
log_reg_predict[log_reg_predict_probs>.5] = 1
log_reg_predict_table <- table(predicted_condition = log_reg_predict, true_condition = testing_data$shares)

# ANN 

library(nnet)
ann_model <-  nnet((as.factor(shares))~., training_data, size = 5)
ann_predict <- predict(ann_model, subset(testing_data, select = -shares), type="class")
ann_table <- table(predicted_condition = ann_predict, true_condition = testing_data$shares)



# ann trail neuralnet library

# library(neuralnet)
# n <- names(training_data)
# f <- as.formula(paste("shares ~", paste(n[!n %in% "shares"], collapse = " + ")))
# dd <- data.matrix(training_data)
# nn <- neuralnet(f, data=dd, hidden= c(30),stepmax=20000,linear.output=FALSE ,err.fct="sse")
# plot(nn)
# tnn <- compute(nn, testing_data[,1:58])
# tnresult <- ifelse(tnn$net.result>0.5,1,0)
# mean(tnresult != testing_data$class)
# table(tnresult , testing_data$shares)
# 



temp <- dt_predict_table[,1]
dt_predict_table[,1] <- dt_predict_table[,2]
dt_predict_table[,2] <- temp
temp <- dt_predict_table[1,]
dt_predict_table[1,] <- dt_predict_table[2,]
dt_predict_table[2,] <- temp

rownames(dt_predict_table) <- c(1,0)
colnames(dt_predict_table) <- c(1,0)


temp <- nb_predict_table[,1]
nb_predict_table[,1] <- nb_predict_table[,2]
nb_predict_table[,2] <- temp
temp <- nb_predict_table[1,]
nb_predict_table[1,] <- nb_predict_table[2,]
nb_predict_table[2,] <- temp

rownames(nb_predict_table) <- c(1,0)
colnames(nb_predict_table) <- c(1,0)



temp <- log_reg_predict_table[,1]
log_reg_predict_table[,1] <- log_reg_predict_table[,2]
log_reg_predict_table[,2] <- temp
temp <- log_reg_predict_table[1,]
log_reg_predict_table[1,] <- log_reg_predict_table[2,]
log_reg_predict_table[2,] <- temp

rownames(log_reg_predict_table) <- c(1,0)
colnames(log_reg_predict_table) <- c(1,0)


temp <- ann_table[,1]
ann_table[,1] <- ann_table[,2]
ann_table[,2] <- temp
temp <- ann_table[1,]
ann_table[1,] <- ann_table[2,]
ann_table[2,] <- temp

rownames(ann_table) <- c(1,0)
colnames(ann_table) <- c(1,0)



dt_precision <- dt_predict_table[1,1]/(dt_predict_table[1,1]+dt_predict_table[1,2])
dt_recall <- dt_predict_table[1,1]/(dt_predict_table[1,1]+dt_predict_table[2,1])
dt_accuracy <- (dt_predict_table[1,1]+dt_predict_table[2,2])/sum(dt_predict_table)

nb_precision <- nb_predict_table[1,1]/(nb_predict_table[1,1]+nb_predict_table[1,2])
nb_recall <- nb_predict_table[1,1]/(nb_predict_table[1,1]+nb_predict_table[2,1])
nb_accuracy <- (nb_predict_table[1,1]+nb_predict_table[2,2])/sum(nb_predict_table)

log_reg_precision <- log_reg_predict_table[1,1]/(log_reg_predict_table[1,1]+log_reg_predict_table[1,2])
log_reg_recall <- log_reg_predict_table[1,1]/(log_reg_predict_table[1,1]+log_reg_predict_table[2,1])
log_reg_accuracy <- (log_reg_predict_table[1,1]+log_reg_predict_table[2,2])/sum(log_reg_predict_table)

ann_precision <- ann_table[1,1]/(ann_table[1,1]+ann_table[1,2])
ann_recall <- ann_table[1,1]/(ann_table[1,1]+ann_table[2,1])
ann_accuracy <- (ann_table[1,1]+ann_table[2,2])/sum(ann_table)


dt_predict_table
print("Decision Tree")
print(paste0("Precision = ", dt_precision))
print(paste0("Recall = ", dt_recall))
print(paste0("Accuracy = ", dt_accuracy))


nb_predict_table
print("Naive Bayes")
print(paste0("Precision = ", nb_precision))
print(paste0("Recall = ", nb_recall))
print(paste0("Accuracy = ", nb_accuracy))

log_reg_predict_table
print("Logistic Regression")
print(paste0("Precision = ", log_reg_precision))
print(paste0("Recall = ", log_reg_recall))
print(paste0("Accuracy = ", log_reg_accuracy))


ann_table
print("ANN")
print(paste0("Precision = ", ann_precision))
print(paste0("Recall = ", ann_recall))
print(paste0("Accuracy = ", ann_accuracy))



