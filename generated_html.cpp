#include <string>

extern "C" {
  const char* get_html() {
      return R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<body>

    <script>
document.addEventListener("DOMContentLoaded", function () {
  console.log("Hello World");
});

</script>
</body>

</html>)";
  }
}
