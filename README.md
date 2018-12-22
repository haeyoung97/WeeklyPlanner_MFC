# WeeklyPlanner_MFC_2018
## 화면구성

###메인화면 

  1) Left  - (사용자의 프로필 사진 설정, 목표 설정 - edit control{변경, 수정 버튼}
    
     > Profile Panel
     
       * Profile Image & Profile Message
      
     > Sound Panel
      
       * Play & Stop & Prev & Next
  
  2) Center
  
    > 체크박스 { Daily 시간, 할 일}
    
      * 할 일 추가 form 설정
      
    > 프로그레스바 {그날의 목표 달성치}
    
      
  3) Right
  
    > D-Day
        > D-day 설정 - 날짜의 의미 ex> 생일, 개학, 방학, 수능 등등
      
        * D-day 추가, 삭제, 수정
        
        > 모달대화상자 - 마감 날짜, 제목, 내용(메모) ( + 아이콘 )
        
          
  4) 이전 기록 보기
  
    > TodoList DB 이용
      
       * 특정 날짜의 데이터를 가져와 이전 일정 및 이전 달성률에 대해 보여준다.
  
##DB 구성
  > DB 연결 방법
  
  > DB Table 구성 
  
    1. MySQL WorkBench 설치
    2. ScheMas 생성 : weeklyplanner 이름으로 생성
    3. Table 생성
       1) TodoList DB
          table name : todolist
          Charset/Collation : utf8/utf8_bin
          
           < Column 1 >
             ColumnName : date
             Datatype : TIMESTAMP
             Not Null : check
             Default/Expression : CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
             
           < Column 2 >
             ColumnName : isDone
             Datatype : TINYINT(4)
             Default/Expression : NULL
             
           < Column 3 >
             ColumnName : Todolist
             Datatype : VARCHAR(45)
             Default/Expression : NULL
             
             
       2) DDay DB
           table name : dday
           Charset/Collation : utf8/utf8_bin
           
           < Column 1 >
             ColumnName : Date
             Datatype : VARCHAR(11)
             Not Null : check
             
           < Column 2 >
             ColumnName : Title
             Datatype : VARCHAR(45)
             Not Null : check


       3) Profile DB  
           table name : profile
           Charset/Collation : utf8mb4/utf8mb4_bin
           
           < Column 1 >
              ColumnName : path
              Datatype : VARCHAR(100)
              Not Null : check
              
           < Column 2 >
              ColumnName : message
              Datatype : VARCHAR(45)
              Not Null : check
              
              
       4) Sound DB
           table name : sound
           Charset/Collation : utf8mb4/utf8mb4_bin
           
           < Column 1 >
             ColumnName : soundPath
             Datatype : VARCHAR(100)
             Not Null : check
             
           < Column 2 >
             ColumnName : soundName
             Datatype : VARCHAR(100)
             Not Null : check


##ODBC 연결

    1. 'ODBC 데이터 원본(32비트)' 검색 후 실행
    
    2. 시스템 DSN 클릭
    
    3. 추가(D) 버튼 클릭
    
    4. MySQL ODBC 5.3 Unicode Driver 선택 후 마침 버튼 클릭
    
    5. Data Source Name : WeeklyPlanner 입력
    
    6. table 생성할 때 썼던 서버 주소와 port 번호, user 이름, 비밀번호 입력
    
    7. Database 콤보박스에서 이전에 만든 Schemas 선택
    
    8. OK 버튼 클릭
