<?php

class ArrayDiff
{
    public array $added = [];
    public array $removed = [];
}

function diff_matches(array $prev, array $current): ArrayDiff
{
    $diff = new ArrayDiff();
    $diff->added = array_diff_key($current, $prev);
    $diff->removed = array_diff_key($prev, $current);
    return $diff;
}

$path = new DirectoryIterator("../archpage");
$dirs = [];
foreach ($path as $fileinfo) {
    if ($fileinfo->isFile() && $fileinfo->getExtension() === 'html'){
        $dirs[] = $fileinfo->getPath() . '/' . $fileinfo->getFilename();
    }
}
sort($dirs);
$current = [];
$prev = [];
foreach ($dirs as $dir) {
    echo "file: $dir\n";
    $file = file_get_contents($dir);
    preg_match_all('#<code>([a-zA-Z\.]+?)</code>#', $file, $matches);
    if (empty($prev)) {
        $prev = array_flip($matches[1]);
        $current = $prev;
        echo "initial: " . count($current) . "\n";
        continue;
    }
    $prev = $current;
    $current = array_flip($matches[1]);
    //echo count($current) . "\n";
    $diff = diff_matches($prev, $current);
    foreach ($diff->added as $key => $value) {
        echo "added: $key\n";
    }
    foreach ($diff->removed as $key => $value) {
        echo "removed: $key\n";
    }
}
