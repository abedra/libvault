create database vault;

create table things(
    id serial primary key,
    name text unique not null,
    created_at timestamp not null default now()
);

insert into things(name) values ('test');

select * from things;
