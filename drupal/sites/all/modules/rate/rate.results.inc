<?php
// $Id: rate.results.inc,v 1.4 2010/10/17 12:16:46 mauritsl Exp $

/**
 * Generate the rate results page.
 *
 * @param object $node
 * @return string
 */
function rate_results_page($node) {
  $output = '';

  $widgets = rate_get_active_widgets('node', $node->type);
  foreach ($widgets as $widget) {
    $output .= '<h3>' . check_plain($widget->title) . '</h3>';
    $votingapi_results = votingapi_select_results(array(
      'content_type' => 'node',
      'content_id' => $node->nid,
      'tag' => $widget->tag,
      'value_type' => $widget->value_type,
    ));
    $count = 0;
    $average = 0;
    $sum = 0;
    foreach ($votingapi_results as $result) {
      ${$result['function']} = $result['value'];
    }
    $output .= '<p>';
    $output .= t('Vote count: @count', array('@count' => $count)) . ' ';
    if ($widget->value_type != 'option') {
      $output .= t('Average: @avg', array('@avg' => round($average, 2))) . ' ';
    }
    if ($widget->value_type == 'points') {
      $output .= t('Points: @points', array('@points' => $sum));
    }
    $output .= '</p>';

    // Display a table with vote counts per button.
    if (count($widget->options) > 1) {
      $rows = array();
      foreach ($widget->options as $option) {
        $sql = 'SELECT COUNT(*) as count
        FROM {votingapi_vote} vv
        WHERE vv.content_type = \'%s\'
        AND vv.content_id = %d
        AND vv.value_type = \'%s\'
        AND vv.value = \'%s\'
        AND vv.tag = \'%s\'
        AND vv.timestamp > %d';
        $res = db_query($sql, 'node', $node->nid, $widget->value_type, $option[0], $widget->tag);
        $total_count = db_result($res);
        $title = $widget->translate ? t($option[1]) : $option[1];
        $rows[] = array($title, $total_count);
      }
      $header = array(t('Button'), t('Votes'));
      $output .= '<h4>' . t('Total votes per button') . '</h4>';
      $output .= theme('table', $header, $rows);
    }

    // Display a graph if the chart module is enabled.
    if (module_exists('chart')) {
      $chart = array(
        '#chart_id' => 'rate_node_' . $node->nid . '_' . $widget->name,
        '#type' => CHART_TYPE_BAR_V_GROUPED,
        '#size' => chart_size(500, 200),
        '#adjust_resolution' => FALSE,
        '#data' => array(),
        '#mixed_axis_labels' => array(
          CHART_AXIS_Y_LEFT => array(),
          CHART_AXIS_X_BOTTOM => array(),
        ),
      );
      $sql = 'SELECT FLOOR((%d - vv.timestamp) / 86400) as days_ago, vv.value, COUNT(*) as count
      FROM {votingapi_vote} vv
      WHERE vv.content_type = \'%s\'
      AND vv.content_id = %d
      AND vv.value_type = \'%s\'
      AND vv.tag = \'%s\'
      AND vv.timestamp > %d
      GROUP BY 1, 2';
      $end_of_day = mktime(23, 59, 59, (int) date('m'), (int) date('d'), (int) date('Y'));
      $min_timestamp = $end_of_day - (86400 * 30);
      $res = db_query($sql, $end_of_day, 'node', $node->nid, $widget->value_type, $widget->tag, $min_timestamp);
      $results = array();
      $oldest = 7; // Display at least 7 days.
      while ($rec = db_fetch_array($res)) {
        $oldest = max($oldest, $rec['days_ago']);
        $results[$rec['days_ago'] . ':' . $rec['value']] = $rec['count'];
      }
      if ($results) {
        $max_count = 0;
        foreach ($widget->options as $option) {
          $value = $option[0];
          $title = $widget->translate ? t($option[1]) : $option[1];
          $chart['#data'][$title] = array();
          for ($i = $oldest; $i >= 0; --$i) {
            $count = isset($results["$i:$value"]) ? $results["$i:$value"] : 0;
            $count = max(0.1, $count);
            $chart['#data'][$title][] = (int) $count;
            $max_count = max($max_count, $count);
          }
        }
        // Workaround the Chart API behaviour that lines with 0's are hidden. Apply a minimum of 1.
        foreach ($chart['#data'] as $title => $counts) {
          for ($i = 0; $i < count($counts); ++$i) {
            $chart['#data'][$title][$i] = max(1, round(($counts[$i] / $max_count) * 100));
          }
        }
        for ($i = $oldest; $i >= 0; --$i) {
          $label = date('j', time() - ($i * 86400));
          $chart['#mixed_axis_labels'][CHART_AXIS_X_BOTTOM][1][] = chart_mixed_axis_label($label);
        }
        // Define colors for the bars. The first color is green, second is red. This is for
        // thumbs up / down voting (the first is up, which needs to be green).
        $colors = array(
          '01df01', 'ff0000', '0101df', 'ffbc00', '01dfd6',
          'b404ae', '8a4b08', '38610b', '610b38', '610b5e',
        );
        $x = 0;
        $chart['#bar_size'] = chart_bar_size(ceil(20 / count($chart['#data'])), 0);
        foreach ($chart['#data'] as $title => $data) {
          $chart['#legends'][] = $title;
          $chart['#data_colors'][] = $colors[$x % count($colors)];
          ++$x;
        }
        $chart['#mixed_axis_labels'][CHART_AXIS_Y_LEFT][0][] = chart_mixed_axis_range_label(0, $max_count);
        $output .= '<h4>' . t('Votes in the last @num days', array('@num' => $oldest)) . '</h4>';
        $output .= '<p>' . t('Showed results are votes per day per button. The horizontal axis shows the days of the month. The vertical axis shows the number of votes.') . '</p>';
        $output .= chart_render($chart);
      }
    }
  }

  return $output;
}