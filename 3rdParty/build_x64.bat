
@REM  We're using eigen as header-only
@REM  SFML contains windows binaries
@REM  TBB contains windows binaries


@REM build POCO
pushd poco-1.6.1
call buildwin 140 build shared both x64 nosamples notests devenv
popd
