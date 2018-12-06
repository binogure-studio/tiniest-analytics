def can_build(platform):
  return platform=="x11" or platform=="windows"

def configure(env):
  env.Append(CPPPATH=["#modules/googleanalytics/libcurl/public/"])
  if env["platform"]== "x11":
    env.Append(LIBS=["libcurl"])
    if env["bits"]=="32":
      env.Append(RPATH=[env.Literal('\\$$ORIGIN/linux32')])
      env.Append(LIBPATH=["#modules/googleanalytics/libcurl/linux32"])
    else: # 64 bit
      env.Append(RPATH=[env.Literal('\\$$ORIGIN/linux64')])
      env.Append(LIBPATH=["#modules/googleanalytics/libcurl/linux64"])
  elif env["platform"] == "windows":
    # mostly VisualStudio
    if env["CC"] == "cl":
      env.Append(LINKFLAGS=["libcurl.lib"])
      if env["bits"]=="32":
        env.Append(LIBPATH=["#modules/googleanalytics/libcurl/win32"])
      else: # 64 bit
        env.Append(LIBPATH=["#modules/googleanalytics/libcurl/win64"])
    # mostly "gcc"
    else:
      env.Append(LIBS=["libcurl"])
      if env["bits"]=="32":
        env.Append(LIBPATH=["#modules/googleanalytics/libcurl/win32"])
      else: # 64 bit
        env.Append(LIBPATH=["#modules/googleanalytics/libcurl/win64"])
