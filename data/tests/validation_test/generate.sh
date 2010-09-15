echo '<?xml version="1.0" encoding="ISO-8859-1" standalone="no" ?>
<profiles>' > "$1"

test_names=$(find ./out/orders -type f | awk -F "/" '{print $NF}' | cut -d '.' -f 1)
for test_name in $test_names
do

echo "<profile name=\"${test_name}\" password=\"\" supervision=\"true\">
    <rights>
      <readonly/>
      <readwrite>
        <side id=\"1\"/>
        <side id=\"2\"/>
        <side id=\"3\"/>
      </readwrite>
    </rights>
  </profile>" >> "$1"
done

echo '</profiles>' >> "$1"