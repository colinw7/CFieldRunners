#include <App.h>
#include <Canvas.h>

#include <CQImage.h>

#include <QApplication>

int
main(int argc, char **argv)
{
  QApplication qapp(argc, argv);

  CQImage::setPrototype();

  std::string initMap;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      auto arg = std::string(&argv[i][1]);

      if (arg == "map") {
        ++i;

        if (i < argc)
          initMap = argv[i];
      }
      else
        std::cerr << "Invalid arg '" << argv[i] << "'\n";
    }
  }

  auto *app = new CQGLFieldRunners::App;

  if (initMap != "")
    app->canvas()->setInitMap(initMap);

  app->init();

  app->show();

  qapp.exec();

  return 0;
}
