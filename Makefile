test:
        rm -rf InfiniBandServerProject
        git clone https://github.com/wohin100/InfiniBandServerProject
        mkdir InfiniBandServerProject/build
        cd InfiniBandServerProject/build; ls; cmake ..; make; ./InfiniBandServer 10.112.51.157 80
