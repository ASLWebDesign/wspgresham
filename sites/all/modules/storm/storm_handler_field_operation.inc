<?php
/**
 * @file
 * Field handler to present a link node edit.
 */
class storm_handler_field_operation extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'node', 'field' => 'uid');
    $this->additional_fields['type'] = array('table' => 'node', 'field' => 'type');
    $this->additional_fields['format'] = array('table' => 'node_revisions', 'field' => 'format');
  }

  function option_definition() {
    // $options = parent::option_definition();
    $options['display_add_icon'] = array('default' => TRUE);
    $options['display_icons'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    // parent::options_form($form, $form_state);
    $form['display_icons'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display icon links'),
      '#description' => t('Display a icon or a text link for edit, delete and add node links.'),
      '#default_value' => $this->options['display_icons'],
    );
    $form['display_add_icon'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a node add icon in the views header?'),
      '#default_value' => $this->options['display_add_icon'],
    );
  }

  function render($values) {
    global $user;
    // ensure user has access to edit this node.
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->uid = $values->{$this->aliases['uid']};
    $node->type = $values->{$this->aliases['type']};
    $node->format = $values->{$this->aliases['format']};
    // TODO add stormorganization_nid and assigned_nid!
    $node->status = 1; // unpublished nodes ignore access control
    if ($this->options['display_icons']) {
      $value = "";
      $value .= storm_icon_edit_node($node, $_GET);
      if (!empty($value)) {
        $value .= '&nbsp;';
      }
      $value .= storm_icon_delete_node($node, $_GET);

      return $value;
    }
    else {
      $value = "";
      if (node_access('update', $node)) {
        $value .= l(t('edit'), "node/$node->nid/edit", array('query' => drupal_get_destination()));
      }
      if (node_access('delete', $node)) {
        if (!empty($value)) {
          $value .= '&nbsp;|&nbsp;';
        }
        $value .= l(t('delete'), "node/$node->nid/delete", array('query' => drupal_get_destination()));
      }

      return $value;

    }
  }
}

