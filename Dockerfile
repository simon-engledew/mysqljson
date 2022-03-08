FROM debian:11.2-slim AS build

RUN apt-get update && apt-get install --no-install-recommends -y curl ca-certificates

WORKDIR /workspace/mysql-server/boost

RUN curl -L http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz -O && \
  tar xfz boost_1_59_0.tar.gz

WORKDIR /workspace/mysql-server

RUN curl -L https://github.com/mysql/mysql-server/archive/refs/tags/mysql-5.7.37.tar.gz | \
  tar xvpz --strip-components=1

RUN apt-get update && apt-get install --no-install-recommends -y build-essential cmake libssl-dev libncurses5-dev bison pkg-config

RUN cmake . -DCMAKE_BUILD_TYPE=Release -DWITH_BOOST=boost

RUN make sql binlog rpl master slave

RUN echo 'MYSQL_ADD_EXECUTABLE(mysqljson mysqljson.cc)' >> sql/CMakeLists.txt \
    && echo 'TARGET_LINK_LIBRARIES(mysqljson sql binlog rpl master slave)' >> sql/CMakeLists.txt

COPY src/ ./

RUN cmake . -DCMAKE_BUILD_TYPE=Release -DWITH_BOOST=boost

RUN make mysqljson

RUN strip sql/mysqljson

FROM debian:11.2-slim

COPY --from=build /workspace/mysql-server/sql/mysqljson /usr/bin/mysqljson

ENTRYPOINT [ "/usr/bin/mysqljson" ]
