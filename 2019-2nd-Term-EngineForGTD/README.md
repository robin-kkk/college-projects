# Engine For GTD <!-- omit in toc -->

- Course: Software Application
- Language: Python

This project was to develop an engine, which could analyze a specific dataset.

---

#### Table of contents <!-- omit in toc -->

- [Overview](#overview)
  - [Goal](#goal)
  - [Dataset](#dataset)
  - [Analysis Environment](#analysis-environment)
- [Preprocessing](#preprocessing)
  - [Basic Analysis](#basic-analysis)
  - [Missing Values](#missing-values)
  - [Categorical Variables](#categorical-variables)
- [Dimensionality Reduction](#dimensionality-reduction)
  - [Multiple Correspondence Analysis](#multiple-correspondence-analysis)
  - [Examples of MCA](#examples-of-mca)
- [Clustering: K-Means](#clustering-k-means)
  - [How to select K](#how-to-select-k)
  - [Examples of K-Means](#examples-of-k-means)
- [Engine Development](#engine-development)
  - [Interface](#interface)
  - [Sequence Diagram](#sequence-diagram)
  - [Result](#result)
- [Conclusion](#conclusion)

---

## Overview

### Goal

The project aims to develop engines that carry out cluster analysis and visualize the consequences of terrorism worldwide in an aggressive way, and to provide data that can help predict future terrorism through these visualization results.

### Dataset

The analysis target is the [Global Terrorism Database (GTD)](https://www.kaggle.com/START-UMD/gtd) managed by the National Consortium for the [Study of Terrors and Responses to Terrorism (START)](https://www.start.umd.edu/data-tools/global-terrorism-database-gtd), which includes more than 180,000 terrorist incidents from the 1970s to 2017. The dataset consists of a total of 135 variables and 181,601 events, and the meaning of each parameter was interpreted based on the codebook provided by the GTD.

### Analysis Environment

- **Language.** Python
- **Modules.** Pandas, Numpy, Matplotlib, Prince, Seaborn, Folium
- **Tools.** Google Coraboratory

## Preprocessing

### Basic Analysis

The dataset has 135 variables, but the meaning of the variables was many overlapping. For example, the `weaptype1` variable, which means a type of the used weapon, has 13 values as categorical variables. This variable stores the category code number, but the `weaptype1_txt` variable stores the category name as a string, not a number. Because of this separate storage of category code numbers and category names, the total number of variables, excluding overlapping variables, is 107.

One event can use more than one weapon, and there can be more than one target of attack. For example, there are three variables that mean target, `targettype1`, `targettype2`, `targettype3`. However, there is no more than one target to attack in all cases, so a null value exists. If the number of such missing cells accounted for more than half of the total, I excluded the variables.

Variables required for cluster analysis should be those that help identify attack tendencies. Therefore, variables that are not required for the analysis, such as the country in which the event occurred, latitude and longitude, and date of the event, were excluded. However, the excluded variables can be used again for visualization. The following is a list of variables to be used in practice.

| Variable      | Description |
| -------------:|:----------- |
| extended      | 1: it was continued more than 24 hours, 0: less than |
| suicide       | 1: it had a suicide attack, 0: hadn't |
| attacktype1   | The attack type has 9 categories. |
| targtype1     | The target type has 22 categories, each classified by target subtype. |
| targsubtype1  | The target subtype has a total 111 categories, each classified by weapon subtype. |
| weaptype1     | The weapon type has 13 categories. |
| weapsubtype1  | The weapon subtype has a total 27 categories. |

### Missing Values

In some cases, even the required variables do not have a value. According to the codebook provided, even if the value "Unknown" is not found, the cell is emptied. Alternatively, if the variable is dependent, it will have missing values, for example, if it is an attack target that does not have a subtype in the `targtype1` variable, then the `targsubtype1` variable will have missing values. In this case, each field was replaced with a value that meant "Unknown" or "Not exists".

### Categorical Variables

A list of variables shows that all the variables to be used are categorical variables. Cluster analysis requires that all of these variables be numerical variables first. In this process, one-hot encoding is applied, which describes the size of the category as a dimension of the vector, giving the corresponding category 1 and the rest 0 values. This expressed vector is called a one-hot vector. The following data are examples of one-hot encoding.

- Before changing

| row | attacktype   | targtype    | weaptype    |
|:---:|:------------:|:-----------:|:-----------:|
| **0** | Bombing      | NGO         | Firearms    |
| **1** | Hijacking    | Police      | Chemical    |
| **2** | Infra Attack | Military    | Nuclear     |
| **3** | Hijacking    | NGO         | Nuclear     |

- After changing

| row | Bombing | Hijacking | Infra Attack | NGO | Police | Military | Fireamrs | Chemical | Nuclear |
|:---:|:-------:|:---------:|:------------:|:---:|:------:|:--------:|:--------:|:-------:|:-------:|
| **0** | 1 | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 0 |
| **1** | 0 | 1 | 0 | 0 | 1 | 0 | 0 | 1 | 0 |
| **2** | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 0 | 1 |
| **3** | 0 | 1 | 0 | 1 | 0 | 0 | 0 | 0 | 1 |

Where the one-hot vector for row 0 is [1, 0, 0, 1, 0, 0, 0, 0] Each row represents an event and is expressed as a vector, which can be viewed as a matrix of these vectors.

## Dimensionality Reduction

### Multiple Correspondence Analysis

Correspondence Analysis is an exploratory data analysis technique that attempts to explore their relationships by simultaneously presenting rows and column categories of data in low-dimensional spatial (two-dimensional) points. If there are two categorical variables, it's called Simple Correspondence Analysis, and if there are more than three variables, it is called Multiple Correspondence analysis. Multiple Correspondence Analysis is a technique in which are widely used for cluster analysis for datasets with many categorical variables.

In Chapter 2, the categorical variables were expressed in a matrix consisting of vectors by one-hot encoding. The matrix is 52 dimensions because it has a total of 52 categories from seven variables. However, in order to represent elements of a matrix in coordinates, the vector dimension must be two dimensions. Therefore, dimension reduction should be performed using multiple correspondence analysis.

### Examples of MCA

As a result of multiple response analyses, two factor coordinates are obtained, and each coordinate allows you to look at the distribution of the original data or the distribution of variables on the graph. However, the distribution of variables does not look at the relationship between the existing variables attacktype1 and weaptype1, but at the relationship between the categories of each variable. For example, the relationship between Bombing of attacktype1 and Firearms of weaptype1, not between attacktype1 and weaptype1. This was not a smooth analysis because the size of the categories varies in each variable, and in case the difference in the size was much more, it skewed the data into the category of the small size. In addition, the factor coordinates of variables were not handled since it was the original purpose of performing cluster analysis in the distribution of data.

A total of seven variables were analyzed, and the variables were used in all or in groups. Groups that believe cluster analysis is difficult to perform, such as data being concentrated in only one location on the coordinates were excluded. Below are groups determined to be suitable for cluster analysis.

| Divison  | variables |
|:-:|:---------:|
| Group 4 | attacktype1, weaptype1, weapsubtype1 |
| Group 10 | attacktype1, weaptype1, targtype1 |
| Group 19 | weaptype1, weapsubtype1, targtype1 |

![1](./img/1.png?raw=true)

There are the results of Group 4, Group 10, Group 14 from left to right.

## Clustering: K-Means

### How to select K

Prior to performing cluster analysis, it is sometimes ambiguous as to how many clusters have the best results. Typical ways to solve this problem are [Elbow Method](https://en.wikipedia.org/wiki/Elbow_method_(clustering)) and [Silhouette Method](https://en.wikipedia.org/wiki/Silhouette_(clustering)).

- **Elbow Method**

This technique is a method of finding k to ensure that Error Sum of Square (SSE) in the distance between clusters is minimal. As shown in the example below, a graph, in which the horizontal axis means the number of clusters, and the vertical axis means SSE, shows a point where the SSE decreases rapidly and then slowly. This point is considered the appropriate number of clusters.

![2](./img/2.png?raw=true)

There are the results of Group 4, Group 10, Group 14 from left to right.

The x value of each point shown in the figure above represents the number of clusters. This example shows k = 3. However, cluster analysis of the actual three clusters may sometimes make it difficult to identify the characteristics of the clusters. In such cases, it is common to apply the silhouette technique described below.

- **Silhouette Method**

The silhouette technique is used to measure how well cluster analysis has been done, when the evaluation scale of the cluster needs to be different due to poor results of the Elbow technique, and evaluates the cluster by identifying the cohesion and separation within the cluster.

The silhouette coefficient s(i) for the i-th data is defined as follows.

![3](./img/3.png?raw=true)

a(i) is a value that represents the cohesion of data within the cluster, which is defined as the average distance between the i-th data and the rest of the data in the same cluster. The smaller the distance, the higher the cohesion. b(i) is a value that represents the separation between clusters and is defined as the average distance between the i-th data and all data within the nearest cluster. If the number of clusters is optimized, b(i) has a large value and a(i) has a small value. Therefore, the silhouette coefficient s(i) is a number close to 1.

The following is of group 4

![4](./img/4.png?raw=true)

The left result is when the number of clusters is 3, and the right is 4. When there are three clusters, the silhouette coefficient is slightly higher than four, and when there are four clusters, one cluster is out of sync and the rest shows values close to the average of the silhouette coefficient. At first, it may be judged that cluster numbers are more suitable when there are three clusters, but cluster 3 is a combination of data-poor categories, making it difficult to identify characteristics. In this case, cluster 3 only can be analyzed again.

### Examples of K-Means

The following is the result of a cluster analysis. The number of clusters was specified as 3 for Group 4, 6 for Group 10, and 5 for Group 19. As I said before, clustering can be performed again for a particular cluster, but Group 4 was able to identify the characteristics within the cluster by increasing the number of clusters.

![5](./img/5.png?raw=true)


## Engine Development

### Interface

This engine doesn't use a database and requires no login. For quick analysis, it receives and analyzes only the CSV file and then renders to a result page. Also, it stores the required data in the server local environment.

There are three main results pages.

1. **Missing Value Processing Result Page.** A page that initially analyzes missing values for the dataset, the next page allows the user to view the results and select the variables to use for MCA. Up to four variable groups can be created, so cluster analysis can be automated up to four times.
2. **Cluster Count Selection Page.** Shows the results of MCA for each group of variables passed on the previous page, along with the results of the Elbow method, and allows users to customize the number of clusters for each group. A separate button is implemented to also apply the silhouette method when necessary.
3. **Cluster Analysis Results page.** A page that is rendered after clustering. This page includes an image of the results before and after clustering for each group, a distribution chart of the variables selected within each cluster, a view of data distribution in each cluster marked with a map, and a graph comparing the number of attacks and deaths by country and region (only the top 15). Click on the marker on the map to view detailed information. In addition, data within the cluster can be saved as a CSV file for further analysis.

The development environment is Python3 in language, Flask in framework. The web server was constructed in Linux. a client can analyze easily on the browser.

### Sequence Diagram

![6](./img/6.png?raw=true)

Instead of getting the data from the user and rendering the page immediately, processing the data, receiving requests again, and rendering the page. This part is intended to allow users to continue to view information about the previous page during which time a user has to wait long in case data is big. Also, the page initially shown to the user (index.html) is rendered separately from when the user uploaded the dataset and when the user did not.

### Result

The following are some of the results pages of Group 4 among the three groups that had a good cluster analysis. Group 4's four clusters account for 50.868%, 34.05%, 9.089%, and 5.989% of the total, respectively, and below is a graph of category distribution within each variable and country-specific data comparison. For a distribution chart of categories, even though the size of the category is greater than 6 the result of less than 1% is truncated from the image.

- Cluster 0 of Group 4

![7](./img/7.png?raw=true)

- Comparison of number of attacks and deaths by country in Cluster 0 of Group 4

![8](./img/8.png?raw=true)

Cluster 0 is a collection of data corresponding to terrorist activities using explosive weapons such as bombs, mines, grenades and so on. In this cluster, the data is widely seen in the Middle East, including Iraq, Afghanistan, and Pakistan.

- Cluster 1 of Group 4

![9](./img/9.png?raw=true)

- Comparison of number of attacks and deaths by country in Cluster 1 of Group 4

![10](./img/10.png?raw=true)

Cluster 1 is a set of terrorist activity data that corresponds to armed attacks, assassinations, and abductions using firearms and knives as weapons. In the order of Pakistan, India, Iraq, and Afghanistan, there is much data in the cluster.

- Cluster 2 of Group 4

![11](./img/11.png?raw=true)

- Comparison of number of attacks and deaths by country in Cluster 2 of Group 4

![12](./img/12.png?raw=true)

Cluster 2 is a set of data on terrorist activities against facilities/infrastructure attacks and unknown attacks using an incendiary bomb, sabotage equipment, etc. The cluster accounts for about 9 percent of the total data, so it is a collection of categories that has the small data previously mentioned. Because there is less data in the cluster, the vertical axis of the graph above shows that the range is far less than that of the previous graph. Similar to the prior cluster, there have been many such attacks in Afghanistan, Iraq, and Columbia.

- Cluster 3 of Group 4

![13](./img/13.png?raw=true)

- Comparison of number of attacks and deaths by country in Cluster 3 of Group 4

![14](./img/14.png?raw=true)

Cluster 3 is the smallest of its kind, accounting for roughly 6 percent of the total, and it feels like data that was able to belong to nowhere. This group includes terrorist activities related to kidnappings, armed attacks, and assassinations in fistfights or in the melee using an incendiary bomb. India, Columbia, and Afghanistan have taken place in order, but there is not much difference in the number of attacks in Pakistan, Peru and so on. Iraq's death toll stands out on the graph, and data shows that a high death toll of armed attacks and assassinations has been reported. The death toll was higher than in other countries because of the high number of cases in Iraq. The act of armed assault is similar to that of Cluster 1 but appears to be the last one because the weapons used are not firearms and data is smaller.

So far, we have looked at the analysis results of Group 4. However, each cluster in the group did not always have a clear feature. In particular, if there are many categories with small data in the group, those categories appear to be clustered together. The following is one of these examples.

- Cluster 4 of Group 10

![15](./img/15.png?raw=true)

The distribution of categories within the `targtype1` variable indicates a variety of business, public goods, transportation, etc. The reason for this result is that we have a wide variety of categories of `targtype1` that fall into a high proportion category in both `weaptype1` and `attacktype1`. Cluster 4 in Group 10 can be re-analyzed and segmented, but not separately because the cluster's ratio accounts for 1% of the total. Rather, it's difficult to analyze because there was too little data to define features.

## Conclusion

Finally, because of missing values in data prior to the 2000s, there were many outliers in the factor coordinates when performing cluster analysis on other variables.
It showed limitations in carrying out.

On the other hand, we were able to see the characteristics of terrorist activities in different countries, given that clusters of different countries show different aspects.

---

p.s: If you wanna see a demo video, check [here](https://drive.google.com/open?id=1Yrv4AXG3uHPyC8COCaznTt7x1wR1CuHU).