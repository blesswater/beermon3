# https://www.neonscience.org/dc-time-series-plot-ggplot-r

install.packages("lubridate")
install.packages("ggplot2")
install.packages("scales")
install.packages("gridExtra")
install.packages("ggthemes")

library(lubridate) # for working with dates
library(ggplot2)  # for creating graphs
library(scales)   # to access breaks/formatting functions
library(gridExtra) # for arranging plots

dat <- read.csv("R:\\HomeProjects\\beermon3\\Data\\IceBuc1.csv", header=T)
str(dat)

qplot(x=timestamp, y=Chan0, data=dat)

library(ggplot2)

library(readr)
library(dplyr)
library(ggplot2)
library(scales)
library(quantmod)

dat <- read.csv("R:\\HomeProjects\\beermon3\\Data\\Test1.csv", header=T)
dat$timestamp = as.POSIXct(dat$timestamp, tz='EST')

g1 <- ggplot(data=dat, aes(x=timestamp, y=Chan0, group=1)) +
    geom_line(color='orange') +
    scale_x_datetime( labels=date_format('%H:%M:%S'), breaks='2 hours' ) +
    xlab( 'Time' ) +
    ylab('Temp (F)')

g2 <-  ggplot(data=dat, aes(x=timestamp)) +
    geom_line( aes(y=Chan0), color='orange') +
    geom_line( aes(y=Chan1), color='green') +
    geom_line( aes(y=Chan2), color='red') +
    geom_line( aes(y=Chan3), color='blue') +
    geom_point( aes(y=ThermometerReading), color='black') +
    scale_x_datetime( labels=date_format('%H:%M:%S'), breaks='2 hours' ) +
    xlab( 'Time' ) +
    ylab('Temp (F)')


g3 <- ggplot(data=dat, aes(x=timestamp)) +
    geom_line( aes(y=RefrigTemp), color='blue') +
    geom_line( aes(y=Setpoint), color='red') +
    scale_x_datetime( labels=date_format('%m-%d'), breaks='1 hours' ) +
    xlab( 'Time' ) +
    ylab('Temp (F)')


