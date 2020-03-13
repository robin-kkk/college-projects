# Engine For GTD

- Course: Software Application
- Language: Python

This project was to develop an engine, which could analyze a specific dataset.

(+ 해당 데이터셋을 분석하는 사람들에게 어떤 (새로운) 정보를 전달해줄 수 있는 엔진이어야 한다.)

I chose [`Global Terrorism Database`]() to what I would analyze.

내가 만들 엔진은 테러 사건들을 클러스터링한 다음, 그 결과를 시각화한다.
시각화 내용은 어떤 군집이 국가별로 많이 분포하는지 등 위치 정보를 포함해서 알려주는 것을 목표로 했다.
자세한 과정은 아래와 같다.

- Global Terrorism Database
    - What is it? : organization, goal, ..., etc
    - intro : rows, columns, variable information, ..., etc
        - there's many categorical variables.
        - there's many null values.

- Preprocessing
    - missing values
    - categorical variables
    - dimension reduction for MCA

- Clustering
    - K-means
    - how to select the 'k'

- Visualization
        ++ images
    - category distribution per cluster
    - marker on Map --> for location info
    - country dist per cluster
    - etc...

- Conclusion



